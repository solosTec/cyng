/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_VM_PROXY_H
#define CYNG_VM_PROXY_H

#include <cyng/task/channel.h>

#include <boost/uuid/uuid.hpp>

namespace cyng {

	/**
	 * Proxy class to access a VM instance
	 */
	class vm_proxy
	{
	public:
		vm_proxy();
		vm_proxy(channel_ptr);
		vm_proxy(vm_proxy const&) = default;

		/**
		 * Assign a (new) channel
		 */
		vm_proxy& operator=(channel_ptr);

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

		/**
		 * allows to give external slots a name to 
		 * call with invoke().
		 */
		void set_channel_name(std::string name, std::size_t idx);

	private:
		channel_ptr	vm_;
	};

	/**
	 * extract UUID from channel name
	 */
	boost::uuids::uuid get_tag(channel_ptr);

}
#endif

