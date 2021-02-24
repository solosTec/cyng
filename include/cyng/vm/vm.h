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

#include <tuple>
#include <functional>
#include <array>
#include <type_traits>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/nil_generator.hpp>

#ifdef _DEBUG_VM
#include <cyng/io/ostream.h>
#include <iostream>
#endif


namespace cyng {

	class mesh;

	/**
	 * Simple VM. Not thread safe.
	 */
	class vm_base
	{
	public:
		vm_base(mesh& fab);
		vm_base(mesh& fab, boost::uuids::uuid tag);

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

	protected:
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
				std::function<void(eod)>	//	stop
			>,
			std::tuple<Fns...>
		>;

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
		vm(mesh& fab, Fns... fns)
			: vm(fab, boost::uuids::nil_uuid(), std::forward<Fns>(fns)...)	//	delegate

		{}
		vm(mesh& fab, boost::uuids::uuid tag, Fns... fns)
			: vm_base(fab, tag)
			, sigs_{
				[&](object obj) {	ctx_.load(std::move(obj)); },
				[&](deque_t deq) {	ctx_.load(std::move(deq)); },
				std::bind(&vm::run, this),
				std::bind(&vm::stop, this, std::placeholders::_1),	//	eod
				//	external functions
				std::forward<Fns>(fns)...
			}
			, arity_{ get_arg_count(fns)... }
		{}

	private:
		signatures_t sigs_;
		arity_t const arity_;
	};

}
#endif

