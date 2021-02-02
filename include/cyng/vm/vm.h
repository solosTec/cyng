/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_VM_VM_H
#define DOCC_VM_VM_H

#include <cyng/vm/context.h>
#include <cyng/meta.hpp>
#include <cyng/obj/intrinsics/eod.h>

#include <tuple>
#include <functional>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/nil_generator.hpp>

#ifdef _DEBUG_VM
#include <cyng/io/ostream.h>
#endif


namespace docscript {

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
		{}

	private:
		signatures_t sigs_;
	};

}
#endif

