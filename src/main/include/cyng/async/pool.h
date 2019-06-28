/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_POOL_H
#define CYNG_ASYNC_POOL_H

#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>
#include <boost/assert.hpp>
#include <iostream>

namespace cyng 
{
	namespace async 
	{
		/**
		 * Simple thread pool that takes
		 * advantage of C++11 move semantics
		 */
		class pool
		{
		private:
			//	typedef for a pool of worker threads
			typedef std::vector< std::thread >		container;

			//	don't copy
			pool(pool const&) = delete;

		public:
			/**
			 *	constructor
			 */
			pool();
			virtual ~pool();

			/**
			 * join all threads from the pool
			 */
			void join_all();

			/**
			 * Build up thread pool.
			 * This method is not thread safe!
			 * 
			 * @param f method to build up the thread
			 * @param count final size of thread pool
			 */
			template < typename F, typename... Args >
			void start( F f, std::size_t count, Args&&... args )
			{
				if (!running_.test_and_set())
				{
					BOOST_ASSERT(workers_.empty());
					BOOST_ASSERT_MSG(count != 0, "empty thread pool");
					workers_.reserve(count);
					while (count-- != 0)
					{
// 						std::cout << "pool::start( " 
// 						<< count
// 						<< ")"
// 						<< std::endl;					
						workers_.emplace_back(f, args...);
					}
				}
			}
			
			/**
			 * Not thread safe.
			 * 
			 * @return true if thread pool is running
			 */
			bool is_running() const;

			/**
			 * Not thread safe.
			 * 
			 * @return thread pool size
			 */
			std::size_t size() const;

		private:
			container	workers_;
			std::atomic_flag running_ = ATOMIC_FLAG_INIT;
		};

	}
}

#endif	//	CYNG_ASYNC_POOL_H

