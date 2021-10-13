/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_VM_H
#define CYNG_VM_VM_H

#include <cyng/vm/context.h>
#include <cyng/meta.hpp>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/task/channel.h>
#include <cyng/obj/function_cast.hpp>
#include <cyng/obj/factory.hpp>

#include <tuple>
#include <functional>
#include <array>
#include <type_traits>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/algorithm/string.hpp>

#ifdef _DEBUG_VM
#include <cyng/io/ostream.h>
#include <iostream>
#endif


namespace cyng {

	namespace {
		template <std::size_t N>
		struct invoke_r_helper
		{
			template <typename Tpl>
			static void call(std::size_t idx, tuple_t const& msg, Tpl& tpl, context& ctx) {
				using F = typename std::tuple_element<N, Tpl>::type;
#ifdef _DEBUG_TEST
				std::cout << "invoke_r: " << idx << ", " << N << std::endl;
#endif
				if (idx == N) {
					using R = typename F::result_type;
					if constexpr (std::is_same<R, void>::value) {
						function_call<F>(std::get<N>(tpl), msg);
						ctx.push(cyng::make_object());	//	void => null
					}
					else {
						ctx.push(cyng::make_object<R>(function_call<F>(std::get<N>(tpl), msg)));
					}
					return;
				}
				invoke_r_helper<N - 1>::call(idx, msg, tpl, ctx);
			}
		};

		template <>
		struct invoke_r_helper<0>
		{
			template <typename Tpl>
			static void call(std::size_t idx, tuple_t const& msg, Tpl& tpl, context& ctx) {

				using F = typename std::tuple_element<0, Tpl>::type;
				using R = typename F::result_type;
#ifdef _DEBUG_TEST
				std::cout << "invoke_r: " << idx << ", " << 0 << std::endl;
#endif
				if constexpr (std::is_same<R, void>::value) {
					function_call<F>(std::get<0>(tpl), msg);
					ctx.push(cyng::make_object());	//	void => null
				}
				else {
					ctx.push(cyng::make_object<R>(function_call<F>(std::get<0>(tpl), msg)));
				}
			}
		};
	}

	//
	//	forward declaration
	//
	class mesh;

	/**
	 * Simple VM. Not thread safe.
	 */
	class vm_base
	{
	public:
		vm_base(channel_weak, mesh& fab);
		vm_base(channel_weak, mesh& fab, boost::uuids::uuid tag);

	protected:

		/**
		 * slots are stopped
		 */
		void stop(eod);

		/**
		 * execute available instructions
		 */
		void run();
		void exec(object const& obj);
		void exec(op);

		/**
		 * call a function over a channel
		 */
		virtual void invoke() = 0;
		virtual void invoke_r() = 0;

	private:
		/**
		 * push UUID on stack
		 */
		void ident();
		void forward();
		void resolve();

	protected:
		channel_weak channel_;
		/**
		 * So VM is able to post messages to all receivers
		 * in the same realm.
		 */
		mesh& mesh_;
		context ctx_;
		boost::uuids::uuid const parent_;
	};

	/**
	 * helper template to compute the arity of
	 * a function object
	 */
	template <typename R, typename ... Args>
	constexpr size_t get_arg_count(R(*f)(Args ...))	{
		return sizeof...(Args);
	}
	template <typename R, typename ... Args>
	constexpr size_t get_arg_count(std::function<R(Args...)>) {
		return sizeof...(Args);
	}

	/**
	 * The VM contains a list of internal function objects and and optional
	 * list of external functions.
	 * 
	 * @tparam TPL external function list
	 */
	template <typename... Fns>
	class vm : public vm_base
	{
		template <typename T >
		friend class task;

		using signatures_t = tmp::tuple_cat_t<
			std::tuple<
				std::function<void(object)>,
				std::function<void(deque_t)>,
				std::function<void()>,	//	run
				std::function<void(std::string, std::size_t)>,	//	set_channel_name
				std::function<void(eod)>	//	stop
			>,
			std::tuple<Fns...>
		>;

		constexpr static std::size_t func_count = sizeof...(Fns);

		/**
		 * The offset marks the beginning of the external functions
		 * in the signatures_t tuple.
		 */
		constexpr static std::size_t offset = std::tuple_size_v< signatures_t > - sizeof...(Fns);

		/**
		 * array with the arity of all external functions
		 */
		using arity_t = std::array<std::size_t, sizeof...(Fns)>;

	public:
		vm(channel_weak wp, mesh& fab, Fns... fns)
			: vm(wp, fab, boost::uuids::nil_uuid(), std::forward<Fns>(fns)...)	//	delegate
		{}

		vm(channel_weak wp, mesh& fab, boost::uuids::uuid tag, Fns... fns)
			: vm_base(wp, fab, tag)
			, sigs_{
				[&](object obj) {	ctx_.load(std::move(obj)); },
				[&](deque_t deq) {	ctx_.load(std::move(deq)); },
				std::bind(&vm::run, this),
				std::bind(&vm::set_channel_name, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&vm::stop, this, std::placeholders::_1),	//	eod
				//	external functions
				std::forward<Fns>(fns)...
			}
			, arity_{ get_arg_count(fns)... }
		{}

	protected:
		/**
		 * call a function over a channel
		 */
		virtual void invoke() override {

			//
			//	stack :
			// * channel/function name
			// * parameter count
			// * parameters ...
			//
			auto [slot, msg] = ctx_.invoke();
			auto sp = channel_.lock();
			if (sp)	sp->dispatch(slot, std::move(msg));
		}

		/**
		 * direct function call (no channel involved) and put return value
		 * on stack.
		 */
		virtual void invoke_r() override {

			auto [slot, msg] = ctx_.invoke_r();
#ifdef _DEBUG_TEST
			std::cout << "function count: " << offset + func_count << std::endl;
#endif
			//invoke_r_helper<offset + func_count - 1>::call(offset + slot, msg, sigs_, ctx_);
			invoke_r_helper<offset + func_count - 1>::call(slot, msg, sigs_, ctx_);
		}

	private:
		/**
		 * give a name to an external slot
		 */
		void set_channel_name(std::string name, std::size_t idx) {
			auto sp = channel_.lock();
			if (sp) {
				sp->set_channel_name(name, offset + idx);
			}
		}

	private:
		signatures_t sigs_;
		arity_t const arity_;
	};

}
#endif

