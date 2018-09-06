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
		 * The callback function will be executed asynchronous in a different thread.
		 *
		 * @param cb callback that provides thread safe (exclusive) access to VM.
		 * 
		 * Function calls can be chained.
		 */
		controller const& access(std::function<void(vm&)> cb) const;

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
		bool is_halted() const;
		
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

		/**
		 * Register a VM procedure.
		 * 
		 * Signature of vm_call is:
		 * @code
		 * void(context&)
		 * @endcode
		 * 
		 * @param name procedure name
		 * @param arity parameter count
		 * @param proc procedure to call 
		 */
		controller const& register_function(std::string name
			, std::size_t arity
			, vm_call proc);

	private:
		/**
		 * Strand to dispatch and synchronize work load
		 */
		mutable dispatcher_t dispatcher_;

		/**
		 * VM implementation
		 */
		mutable vm vm_;
		
		/**
		 * If true machine is halted.
		 */
		std::atomic<bool>	halt_;
		
	};

	/**
	 * @return occurence count of specified op
	 */
	std::size_t op_counter(vector_t const& prg, code op);
}

#endif	//	CYNG_VM_CONTROLLER_H



