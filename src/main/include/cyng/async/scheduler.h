/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_SCHEDULER_H
#define CYNG_ASYNC_SCHEDULER_H

#include <boost/asio.hpp>
#include <cyng/compatibility/io_service.h>
#include <boost/assert.hpp>
#include <cyng/async/pool.h>
#include <cyng/async/policy.h>

namespace cyng 
{
	namespace async 
	{
		/**
		 * Straight forward thread scheduler that
		 * works with a thread pool.
		 */
		class scheduler
		{
		public:
			/**
			 * Thread scheduler is running immediately
			 * after leaving the constructor.
			 */
			scheduler(std::size_t size);
			scheduler();

			/**
			 * io_service_t is not moveable by design.
			 *
			 * @see https://svn.boost.org/trac/boost/ticket/9213
			 */
			scheduler(scheduler&&) = delete;
		
			/**
			 * No copy constructor allowed
			 */
			scheduler(scheduler const&) = delete;
			
			virtual ~scheduler();
			
			/**
			 * This function may be used to obtain the io_service object 
			 * that the scheduler uses to dispatch handlers for asynchronous operations. 
			 */
			inline io_service_t & get_io_service()
			{
				return service_;
			}
		
			/**
			 * @return true if io_service is not stopped
			 */
			inline bool is_running() const
			{
				return !service_.stopped();
			}
			
			/**
			 * shutdown thread pool
			 */
			void stop();
			
			/**
			 *  Invoke the given handler and return immediately. 
			 */
			template < typename H >
			auto invoke(H&& h) -> decltype(std::declval<io_service_t>().post(h))
			{
				return service_.post(h);
			}

			/** 
			 * @return thread pool size
			 */
			std::size_t get_pool_size() const;

		private:
			/**
			 * startup thread pool
			 * 
			 * @param count thread pool size
			 */
			bool start(std::size_t count);
			
			/**
			 * method each thread is running 
			 */
			void run(std::atomic<std::size_t>&);
					
		private:
			io_service_t		service_;
			
			/**
			 * This keeps the service running
			 */
// BOOST_VERSION < 106600
#if (BOOST_ASIO_VERSION < 101200)
			std::unique_ptr<boost::asio::io_service::work>	work_;
#else
			boost::asio::executor_work_guard<io_service_t::executor_type>	work_;
#endif
			
			/**
			 * The thread pool
			 */
			pool 	pool_;
		};

	}
}

#endif	//	CYNG_ASYNC_SCHEDULER_H


