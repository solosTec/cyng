/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_CONTROLLER_H
#define CYNG_VM_CONTROLLER_H

#include <cyng/compatibility/io_service.h>
#include <cyng/vm/vm.h>
#include <cyng/async/policy.h>
#include <cyng/compatibility/async.h>

namespace cyng 
{
	class alloc_stack;
	class call_stack
	{
		friend class alloc_stack;
	public:
		call_stack();
		bool running_in_this_thread() const;
	private:
		mutable std::stack<std::thread::id>	call_stack_;
	};

	class alloc_stack
	{
	public:
		alloc_stack(call_stack const&);
		virtual ~alloc_stack();
	private:
		call_stack const& call_stack_;
	};


	/**
	 *	Wrapper class for VM. All calls into the VM are dispatched
	 *	over a strand object. This allows execution of code 
	 *	in a multithreaded program without the need for explicit locking.
	 *
	 *	It's in the responsibility of the implementor that all calls
	 *	are complete before the destructor is called.
	 */
	class controller
	{
		/**
		 * helper class to provide a move parameter.
		 */
		struct parameter
		{
			mutable vector_t prg_;
			parameter(vector_t&&);
			parameter(parameter const&);
			parameter(parameter&&);
		};

	public:
		controller() = delete;
		controller(io_service_t&, boost::uuids::uuid, std::ostream& = std::cout, std::ostream& = std::cerr);
		
		/**
		 * Execute the specified instructions asynchonously
		 * but wait for the result code (register)
		 * 
		 * @param prg instructions to execute
		 */
 		controller const& run(vector_t&& prg) const;
		
		/**
		 * Execute the specified instructions asynchonously.
		 * 
		 * @param prg instructions to execute
		 * @return this object - allows function chaining
		 */
		controller const& async_run(vector_t&& prg) const;
		
		/**
		 * Halt engine.
		 */
		void halt();
		
		/**
		 * @return VM specific hash based in internal tag
		 */
		std::size_t hash() const noexcept;

		/**
		 * @return VM internal tag
		 */
		boost::uuids::uuid tag() const noexcept;

		/**
		 * @return true if the current thread is executing a handler that was submitted 
		 * from this VM. Otherwise returns false.
		 */
		bool same_thread() const;

	private:
		void execute(vector_t&& prg, async::sync) const;
		void execute(vector_t&& prg, async::detach) const;
		
	private:
#ifdef CYNG_VM_SIMPLE_LOCK
		/**
		 * Boost.Asio dispatcher for async calls
		 */
		io_service_t& dispatcher_;
#else
		/**
		 * Strand to dispatch and synchronize work load
		 */
		mutable dispatcher_t dispatcher_;
#endif

		/**
		 * VM implementation
		 */
		mutable vm vm_;
		
		/**
		 * If true machine is halted.
		 */
		std::atomic<bool>	halt_;
		
		/**
		 * support for sync calls
		 */
		mutable async::mutex	mutex_;

		/**
		 * Handle recursion
		 */
		call_stack	call_stack_;
	};

}

#endif	//	CYNG_VM_CONTROLLER_H



