/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_BASE_TASK_H
#define CYNG_ASYNC_BASE_TASK_H


#include <cyng/async/task_fwd.h>
#include <cyng/intrinsics/sets.h>
#include <cyng/compatibility/io_service.h>
#include <chrono>
#include <iostream>
#include <boost/asio/steady_timer.hpp>

namespace cyng 
{
	namespace async 
	{
		class base_task
		{
		public:
			base_task(mux&);
			
			//	
			//	public task interface:
			//	
			
			/**
			 * run() is called at startup and from timer callback.
			 */
			virtual void run() = 0;

			/**
			 * stop() is called to shutdown a task.
			 *
			 * @param shutdown true if mux is in shutdown mode and task has to signal if termination
			 *	process is complete
			 * @return The number of asynchronous operations that were cancelled.
			 */
			virtual std::size_t stop(bool shutdown) = 0;

			/**
			 * Dispatch message to specified slot. It effectively calls the member function
			 * process() and expands the tuple to a parameter pack with correct data types.
			 */
			virtual void dispatch(std::size_t slot, tuple_t msg) = 0;
			virtual void dispatch(std::string slot, tuple_t msg) = 0;

			/**
			 * gcc requires an implementation even though this
			 * function is virtual. On the other side, this is a 
			 * cool feature to detect pure virtual calls.
			 */
			virtual shared_task get_shared() = 0;

			/*
			 * Path to get the class name of a task and to group
			 * by implementation classes.
			 */
			virtual std::string get_class_name() const = 0;
			
			/**
			 * @return task id
			 */
			std::size_t get_id() const;
			
			template < typename R, typename P >
			void suspend(std::chrono::duration<R, P> d)
			{
				if (shutdown_)	return;

				auto ptr = get_shared();
				timer_.expires_from_now(d);
				timer_.async_wait([ptr](boost::system::error_code const& ec){
					if (ec != boost::asio::error::operation_aborted)
					{
						ptr->run();
					}
				});
			}
			
			void suspend_until(std::chrono::system_clock::time_point);

		protected:
			/**
			 * @return The number of asynchronous operations that were cancelled.
			 */
			std::size_t cancel_timer();

			/**
			 * remove this task from task list.
			 */
			void remove_this();

		public:
			mux& mux_;

		protected:
			const std::size_t id_;
 			boost::asio::steady_timer timer_;
			dispatcher_t dispatcher_;
			bool shutdown_;

		};
		

	}	// async
}

#endif	//	CYNG_ASYNC_BASE_TASK_H




