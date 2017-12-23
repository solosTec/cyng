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
#include <future>

namespace cyng 
{
	namespace async 
	{
		template <typename T>
		class task : public base_task, public std::enable_shared_from_this< task < T > >
		{
			using impl_type = T	;	//	implementation
			using this_type = task < T >;
			
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
				
// 				std::cout << "task<>::stopp(" << get_id() << ")" << std::endl;
				f.wait();
				std::cout << "task<>::stopped(" << get_id() << ")" << std::endl;
				
			}
			
			/**
			 * dispatch() receive messages and dispatch each message 
			 * to the appropriate slot.
			 */
			virtual int dispatch(std::size_t slot, tuple_t const& msg) override
			{
				if (shutdown_)	return 0;
				auto sp = this->shared_from_this();
				dispatcher_.post([this, sp, slot, msg](){
					select_signature<signatures_t>::invoke(impl_, slot, msg);
				});
				return 0;
			}
			
			virtual shared_task get_shared() override
			{
				return this->shared_from_this();
			}
			
		private:
			impl_type	impl_;
			
		};
		

	}	// async
}

#endif	//	CYNG_ASYNC_TASK_HPP




