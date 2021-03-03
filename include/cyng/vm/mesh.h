/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_MESH_H
#define CYNG_VM_MESH_H

#include <cyng/task/channel.h>
#include <cyng/obj/util.hpp>
#include <cyng/task/controller.h>
#include <type_traits>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace cyng {

	/**
	 * Proxy class to access a VM instance
	 */
	class vm_proxy
	{
	public:
		vm_proxy() = delete;
		vm_proxy(channel_ptr);
		vm_proxy(vm_proxy const&) = default;

		/**
		 * run the loaded program
		 */
		void run();

		/**
		 * load a single instruction or value
		 */
		void load(object&&);

		/**
		 * load a bunch of instructions or values at once
		 */
		void load(deque_t&&);

		/**
		 * load the specified arguments and run this as program.
		 */
		template < typename ...Args >
		void execute(Args&&... args) {
			load(make_deque(std::forward<Args>(args)...));
			run();
		}

		/**
		 * stop VM
		 */
		void stop();

		/**
		 * @return the VM tag
		 */
		boost::uuids::uuid get_tag() const;


	private:
		channel_ptr	vm_;
	};

	class controller;
	template <typename... Fns>	class vm;

	/**
	 * Managing VM instances
	 */
	class mesh
	{
		/**
		 * Grant access to controller to enable message postings.
		 */
		template <typename... Fns>
		friend class vm;

	public:
		explicit mesh(controller&) noexcept;

		template <typename... Fns>
		vm_proxy create_proxy(Fns&&... fns) {
			return create_vm(std::forward<Fns>(fns)...);
		}

		template <typename... Fns>
		vm_proxy create_proxy(boost::uuids::uuid parent, Fns&&... fns) {
			return create_vm(parent, std::forward<Fns>(fns)...);
		}

		/**
		 * Search for a VM with the specified tag
		 */
		channel_ptr lookup(boost::uuids::uuid tag);

		/**
		 * get access to task manager
		 */
		controller& get_ctl();

	private:
		template <typename... Fns>
		channel_ptr create_vm(Fns&&... fns) {
			return create_vm(boost::uuids::nil_uuid(), std::forward<Fns>(fns)...);
		}

		template <typename... Fns>
		channel_ptr create_vm(boost::uuids::uuid parent, Fns&&... fns) {

			//
			//	create a uuid
			//
			auto const tag = uuid_rgn_();

			//
			//	create task channel
			//
			using vm_t = vm<typename std::decay<Fns>::type...>;	//	no references
			return ctl_.create_named_channel<vm_t>(boost::uuids::to_string(tag), *this, parent, std::forward<Fns>(fns)...);

		}

	private:
		controller& ctl_;
		boost::uuids::random_generator	uuid_rgn_;
	};

	/**
	 * extract UUID from channel name
	 */
	boost::uuids::uuid get_tag(channel_ptr);

}
#endif

