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

	class vm_proxy;
	template <typename Token>
	void exec(vm_proxy&, Token&&);
	boost::asio::io_context::strand& expose_dispatcher(vm_proxy& pr);

	/**
	 * Proxy class to access a VM instance
	 */
	class vm_proxy
	{
		template <typename Token>
		friend void exec(vm_proxy&, Token&&);
		friend boost::asio::io_context::strand& expose_dispatcher(vm_proxy& pr);

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

	/**
	 * Use the same strand as the channel
	 */
	template <typename Token>
	void exec(vm_proxy& pr, Token&& token) {
		BOOST_ASSERT(pr.vm_);
		exec(*pr.vm_, std::forward<Token>(token));
	}

	boost::asio::io_context::strand& expose_dispatcher(vm_proxy& pr);
}
#endif

