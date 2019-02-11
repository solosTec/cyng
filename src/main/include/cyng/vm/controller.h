/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VM_CONTROLLER_H
#define CYNG_VM_CONTROLLER_H

#include <cyng/vm/controller_fwd.h>
#include <cyng/compatibility/io_service.h>
#include <cyng/vm/vm.h>
#include <cyng/async/policy.h>
#include <cyng/compatibility/async.h>
#include <initializer_list>
#include <thread>

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
			parameter();
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
		 * example:
		 @code
		 async_run(generate_invoke("log.msg.trace", 42) << generate_invoke_unwinded("log.msg.trace", 43));
		 @endcode
		 */
		controller const& async_run(vector_t& prg) const;

		/**
		 * Use an initializer list leads to copies of all parameters.
		 *
		 * example:
		 * 
		 @code
		 async_run({generate_invoke("log.msg.trace", 42), generate_invoke("log.msg.trace", 43)});
		 @endcode
		 */
		controller const& async_run(std::initializer_list<vector_t> prg) const;

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

		/**
		 * Send HALT instruction to VM
		 */
		void halt();

		/**
		 * Wait for VM to stop.
		 */
		template <typename R, typename P>
		bool wait(std::size_t counter, std::chrono::duration<R, P> const& d)
		{
			BOOST_ASSERT_MSG((d * counter) < std::chrono::minutes(5), "unrealistic long waiting time");
			while (!is_halted() && (counter-- != 0)) {
				std::this_thread::sleep_for(d);
			}
			return is_halted();
		}

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
}

#endif	//	CYNG_VM_CONTROLLER_H



