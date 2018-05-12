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
#include <boost/core/demangle.hpp>
#include <boost/algorithm/string.hpp>
#include <memory>
#include <future>
#include <typeinfo>

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
				auto sp = this->shared_from_this();
				dispatcher_.post([this, sp](){
					impl_.run();					
				});
			}
			
			/**
			 * stop() is called to shutdown a task.
			 */
			virtual void stop() override
			{
				if (shutdown_)	return;
				
				//
				// set shutdown flag
				//
				shutdown_ = true;
				
				//
				//	Add one reference to prevent call 
				//	of destructor.
				//
				auto sp = this->shared_from_this();
				
				std::promise<bool> result;
				auto f = result.get_future();
				
				dispatcher_.post([this, sp, &result](){
					
					impl_.stop();
					
					//
					//  Now it's safe to stop the timer.
					//  Before stop() a task could restart the timer again.
					//  It is the responsibility of the implementor to guarantee
					//  this behavior. 
					//
					sp->cancel_timer();
					
					result.set_value(true);
				});
				
// 				std::cout << "task<>::stop(" << get_id() << ")" << std::endl;
				f.wait();
				//std::cout << "task<>::stopped(" << get_id() << ")" << std::endl;
				
			}
			
			/**
			 * dispatch() receive messages and dispatch each message 
			 * to the appropriate slot.
			 */
			virtual void dispatch(std::size_t slot, tuple_t msg) override
			{
				if (shutdown_)	return;
				auto sp = this->shared_from_this();
				dispatcher_.post([this, sp, slot, msg]() {
					switch (select_signature<signatures_t>::invoke(impl_, slot, msg))
					{
					case continuation::TASK_STOP:
						this->shutdown_ = true;
						impl_.stop();
						sp->cancel_timer();
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

				});
			}
			
			virtual shared_task get_shared() override
			{
				return this->shared_from_this();
			}
			
			virtual std::string get_class_name() const
			{
				//	class node::xxxxxxx
				std::vector<std::string> parts;
				auto name = boost::core::demangle(typeid(impl_type).name());
				boost::split(parts, name, boost::is_any_of("\t "));
				
				return parts.empty()
					? "TASK"
					: ((parts.size() > 1) ? parts.at(1)	: parts.at(0));
			}

		private:
			impl_type	impl_;
			
		};
		
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





