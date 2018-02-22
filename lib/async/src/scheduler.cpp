/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/async/scheduler.h>
#include <iostream>


namespace cyng 
{
	namespace async 
	{
		scheduler::scheduler()
			: service_()
#if (BOOST_ASIO_VERSION < 101200)
			, work_(new boost::asio::io_service::work(service_))
#else
			, work_(service_.get_executor())
#endif
			, pool_()
		{
			start(std::thread::hardware_concurrency() * 2);
		}

		scheduler::scheduler(std::size_t size)
			: service_()
#if (BOOST_ASIO_VERSION < 101200)
			, work_(new boost::asio::io_service::work(service_))
#else
			, work_(service_.get_executor())
#endif
			, pool_()
		{
			if (size == 0)
			{
				BOOST_THROW_EXCEPTION(std::invalid_argument("thread pool size"));
			}
			start(size);
		}
		
		scheduler::~scheduler()
		{
			if (is_running())
			{
				stop();
			}
		}
		

		bool scheduler::start(std::size_t size)
		{
			BOOST_ASSERT(size != 0);
			
			if (!pool_.is_running() && size != 0)
			{
				//
				//	counter for running threads
				//
				std::atomic<std::size_t> counter(0);
				
				//
				//	startup all threads
				//
				pool_.start([this, &counter](){
					this->run(counter);
				}, size);
				
				//
				//	wait until all threads are ready
				//
				while(counter != size)
				{
					std::this_thread::yield();
				}
				return true;
			}
			return false;
		}
		
		void scheduler::run(std::atomic<std::size_t>& counter)
		{
			++counter;
			if (is_running())
			{
				
				//std::cout << "scheduler::run( " 
				//<< std::this_thread::get_id() 
				//<< ", "
				//<< counter
				//<< ")"
				//<< std::endl;			
				
				//
				//	Blocks until stop() is called.
				//
				service_.run();
			}
		}

		void scheduler::stop()
		{
			if (is_running())
			{
				//
				//	remove work set
				//
				work_.reset();
				
				//
				//	blocks until all work has finished and there are no more handlers to be dispatched
				//
				while(service_.run() != 0)
				{
					//std::cout << "pending handlers" << std::endl;
					std::this_thread::sleep_for(std::chrono::seconds(1));
				}
				
				service_.stop();
				pool_.join_all();
			}
		}

		std::size_t scheduler::get_pool_size() const
		{
			return pool_.size();
		}

	}
}
