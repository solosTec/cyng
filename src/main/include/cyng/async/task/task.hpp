/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_TASK_HPP
#define CYNG_ASYNC_TASK_HPP


#include <cyng/async/task/base_task.h>
#include <cyng/async/task/task_meta.hpp>
#include <memory>
#include <typeinfo>
#include <thread>
#include <boost/core/demangle.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/core/ignore_unused.hpp>

namespace cyng 
{
	namespace async 
	{
		template <typename T>
		class task : public base_task, public std::enable_shared_from_this< task < T > >
		{
			using impl_type = T;	//	implementation
			using this_type = task < T >;

			template < typename TASK >
			friend auto task_cast(shared_task tp) noexcept->TASK*;

			//	A task implementation class must declare 
			//	a list of message types.
			using signatures_t = typename impl_type::signatures_t;
			
		public:
			template < typename ...Args >
			task(mux& m, Args &&... args)
			: base_task(m)
			, impl_(this, std::forward<Args>(args)...)
			{}
			
			//	
			//	public task interface:
			//	
			
			/**
			 * run() is called at startup and from timer callback.
			 */
			virtual void run() override
			{
				if (shutdown_)	return;
				auto sp{ this->shared_from_this() };
				dispatcher_.post([this, sp](){
					if (!shutdown_)	eval_rc(impl_.run());
				});
			}
			
			/**
			 * stop() is called to shutdown a task.
			 *
			 * @param shutdown true if mux is in shutdown mode and task has to signal if termination
			 *	process is complete
			 * @return The number of asynchronous operations that were cancelled.
			 */
			virtual std::size_t stop(bool shutdown) override
			{
				if (shutdown_)	return 0u;
				
				//
				// set shutdown flag
				//
				shutdown_ = true;
				
				try {
					impl_.stop(shutdown);
				}
				catch(std::exception const& ex) {
					boost::ignore_unused(ex);
				}

				//
				//  Now it's safe to stop the timer.
				//  Before stop() a task could restart the timer again.
				//  It is the responsibility of the implementor to guarantee
				//  this behavior. 
				//	return the number of asynchronous operations that were cancelled.
				//
				return cancel_timer();
			}
			
			/**
			 * dispatch() receive messages and dispatch each message 
			 * to the appropriate slot.
			 */
			virtual void dispatch(std::size_t slot, tuple_t msg) override
			{
				if (shutdown_ || (slot == NO_SLOT))	return;
				auto sp{ this->shared_from_this() };
				dispatcher_.post([this, sp, slot, msg]() {
					if (!shutdown_)	eval_rc(select_signature<signatures_t>::invoke(impl_, slot, msg));
				});
			}

			/**
			 * dispatch() receive messages and dispatch each message 
			 * to the appropriate slot.
			 */
			virtual void dispatch(std::string slot, tuple_t msg) override
			{
				if (!shutdown_)	dispatch(resolve_name(slot), msg);
			}

			/**
			 * Compiler selects the SLOT. 
			 * This is faster at runtime but doesn't work as virtual function.
			 */
			template <std::size_t SLOT>
			void post(tuple_t msg)
			{
				if (!shutdown_)	eval_rc(invoke_slot<impl_type, SLOT>(impl_, msg));
			}
			
			virtual shared_task get_shared() override
			{
				return this->shared_from_this();
			}
			
			virtual std::string get_class_name() const
			{
				return get_class_name_impl();
			}

			static std::string get_class_name_impl()
			{
				//	class node::xxxxxxx
				std::vector<std::string> parts;
				auto name = boost::core::demangle(typeid(impl_type).name());
				boost::split(parts, name, boost::is_any_of("\t "));

				return parts.empty()
					? "TASK"
					: parts.back()
					;
			}

		protected:
			virtual void timeout() override
			{
				if (!shutdown_)	eval_rc(impl_.run());
			}

		private:
			/**
			 * Support for named slots.
			 * Takes a given name and returns the according slot ID.
			 */
			static std::size_t resolve_name(std::string const& name)
			{
				auto pos = slot_names_.find(name);
				return (pos != slot_names_.end())
					? pos->second
					: NO_SLOT
					;
			}

			/**
			 * Evaluates the return code of an applied task function.
			 */
			void eval_rc(continuation rc)
			{
				switch (rc)
				{
				case continuation::TASK_STOP:
					this->shutdown_ = true;
					impl_.stop(false);	//	no system shutdown
					this->cancel_timer();
					remove_this();
					break;
				case continuation::TASK_YIELD:
					std::this_thread::yield();
					break;
				case continuation::TASK_CONTINUE:
				case continuation::TASK_UNDEFINED:
				default:
					break;
				}
			}

		public:
			/**
			 * Optional support for named slots.
			 * Takes a given name and returns the according slot ID.
			 * All you have to do is to add an static initializer for this map
			 *
			 * example:
			 * @code
			 template <>
			 std::map<std::string, std::size_t> async::task<simple>::slot_names_({ {"slot-0", 0} });
			 * @endcode
			 */
			static std::map<std::string, std::size_t>	slot_names_;

		private:
			impl_type	impl_;
			
		};
		
		//
		//	initialize static member
		//
		template <typename T>
		std::map<std::string, std::size_t> task<T>::slot_names_({});

		/**
		 * This cast enables access to the embedded task object
		 */
		template < typename T >
		auto task_cast(shared_task tp) noexcept -> T* 
		{
			auto ptr = std::dynamic_pointer_cast< task<T> >(tp);
			if (ptr)
			{
				return &ptr.get()->impl_;
			}
			return nullptr;
		}

	}	// async
}

#endif	//	CYNG_ASYNC_TASK_HPP





