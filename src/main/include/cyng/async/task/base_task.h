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
// #include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <iostream>

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
			virtual void stop() = 0;
			virtual int dispatch(std::size_t slot, tuple_t const& msg) = 0;
			
			/**
			 * gcc requires an implementation even though this
			 * function is virtual. On the other side, this is a 
			 * cool feature to detect pure virtual calls.
			 */
			virtual shared_task get_shared() = 0;
			
			/**
			 * @return task id
			 */
			std::size_t get_id() const;
			
			template < typename R, typename P >
			void suspend(std::chrono::duration<R, P> d)
			{
				if (shutdown_)	return;

				timer_.expires_from_now(d);
// 				timer_.async_wait(dispatcher_.wrap([this](boost::system::error_code const& ec){
				timer_.async_wait([this](boost::system::error_code const& ec){
					if (ec != boost::asio::error::operation_aborted)
					{
						this->run();
					}
					else 
					{
						std::cout << "cancel timer " << this->get_id() << std::endl;
					}
				});
			}
			
		protected:
			void cancel_timer();
			
		protected:
			mux& mux_;
			const std::size_t id_;
 			boost::asio::steady_timer timer_;
			dispatcher_t dispatcher_;
// 			boost::asio::strand	dispatcher_;
			bool shutdown_;
		};
		

	}	// async
}

#endif	//	CYNG_ASYNC_BASE_TASK_H




