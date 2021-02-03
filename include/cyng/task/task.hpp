/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SCRIPT_TASK_HPP
#define CYNG_SCRIPT_TASK_HPP

#include <cyng/task/channel.h>
#include <cyng/task/registry.h>
#include <cyng/task/task_interface.h>
#include <cyng/obj/object.h>
#include <cyng/obj/function_cast.hpp>
#include <cyng/meta.hpp>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/obj/util.hpp>

namespace cyng {

	namespace {
		template <std::size_t N>
		struct invoke_helper
		{
			template <typename Tpl>
			static void call(std::size_t idx, tuple_t const& msg, Tpl& tpl) {
				using F = typename std::tuple_element<N, Tpl>::type;
				//std::cout << "call: " << idx << ", " << N << std::endl;
				if (idx == N) {
					function_call<F>(std::get<N>(tpl), msg);
					return;
				}
				invoke_helper<N - 1>::call(idx, msg, tpl);
			}
		};

		template <>
		struct invoke_helper<0>
		{
			template <typename Tpl>
			static void call(std::size_t idx, tuple_t const& msg, Tpl& tpl) {

				using F = typename std::tuple_element<0, Tpl>::type;
				//std::cout << "call: " << idx << ", " << 0 << std::endl;
				function_call<F>(std::get<0>(tpl), msg);
			}
		};
	}

	namespace traits {
		/**
		 * Initialize implementation class with a weak reference 
		 * of the channel object. Enables to start a timer.
		 */
		struct weak_ref {};

		/**
		 * No additional parameters are passed to the implementation class
		 */
		struct no_ref {};
	}

	template <typename T>
	class task : public task_interface
	{
		friend class channel;
		friend class registry;
		using seq_t = std::make_index_sequence < std::tuple_size_v<typename T::signatures_t> >;
		constexpr static std::size_t signature_count_ = std::tuple_size<typename T::signatures_t>::value - 1u;

		using stop_fn = std::function<void(eod)>;
		constexpr static std::size_t stop_idx_ = tmp::index<stop_fn, typename T::signatures_t>::value;

	public:
		template < typename ...Args >
		task(traits::no_ref, boost::asio::io_context& ctx, registry* reg, std::size_t id, std::string name, Args &&... args)
			: registry_(reg)
			, id_(id)
			, channel_(std::make_shared<channel>(ctx, this, name))
			, impl_(std::forward<Args>(args)...)
		{ 
			register_this();
		}

		template < typename ...Args >
		task(traits::weak_ref, boost::asio::io_context& ctx, registry* reg, std::size_t id, std::string name, Args &&... args)
			: registry_(reg)
			, id_(id)
			, channel_(std::make_shared<channel>(ctx, this, name))
			, impl_(channel_->weak_from_this(), std::forward<Args>(args)...)
		{
			register_this();
		}

		virtual ~task()
		{
			BOOST_ASSERT(!channel_->is_open());
			//
			//	remove from registry?
			//
		}

		virtual std::size_t get_id() const noexcept override 
		{
			return id_;
		}

		virtual channel_ptr get_channel() const noexcept override
		{
			return channel_->shared_from_this();
		}

	private:
		void register_this()
		{
			registry_->insert(this);
		}

		void unregister_this(destruct_cb cb)
		{
			registry_->remove(id_, cb);
		}

	protected:
		virtual void dispatch(std::size_t slot, tuple_t const& msg) override
		{
			//invoke(slot, msg, seq_t{});
			BOOST_ASSERT(slot <= signature_count_);
			if (slot > signature_count_)	return;
			invoke_helper<signature_count_>::call(slot, msg, impl_.sigs_);

		}
		virtual void stop(destruct_cb cb) override
		{
			//
			//	call the last entry
			//
			//dispatch(signature_count_, tuple_t{});
			dispatch(stop_idx_, cyng::make_tuple(eod{}));

			unregister_this(cb);
		}

	private:
		registry* registry_;
		std::size_t const id_;

		//
		//	Ordering is important.
		//	First the channel have to be initialized 
		//	to get used by the implementation class
		//
		channel_ptr channel_;
		T	impl_;	//!<	implementation class
	};



}

#endif
