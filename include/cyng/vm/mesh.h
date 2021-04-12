/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_MESH_H
#define CYNG_VM_MESH_H

#include <cyng/vm/proxy.h>
#include <cyng/task/controller.h>
#include <type_traits>

#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/nil_generator.hpp>

namespace cyng {

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
			return ctl_.create_named_channel_with_ref<vm_t>(boost::uuids::to_string(tag), *this, parent, std::forward<Fns>(fns)...);

		}

	private:
		controller& ctl_;
		boost::uuids::random_generator	uuid_rgn_;
	};

}
#endif

