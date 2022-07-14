#include <cyng/task/scheduler.h>
#include <algorithm>

#ifdef _DEBUG_TASK
#include <iostream>
#endif

namespace cyng {

	scheduler::scheduler()
		: ctx_(static_cast<std::size_t>(std::thread::hardware_concurrency()) * 2)	//	hint
		, work_(ctx_.get_executor())	//	keep running
		, pool_()
    {
		start(static_cast<std::size_t>(std::thread::hardware_concurrency()) * 2);
	}

	scheduler::scheduler(std::size_t num_threads)
		: ctx_(std::max<std::size_t>(1, num_threads))	//	hint
		, work_(ctx_.get_executor())	//	keep running
		, pool_()
	{
		start(std::max<std::size_t>(1, num_threads));
	}

	scheduler::io_context_t& scheduler::get_ctx()
	{
		return ctx_;
	}

	void scheduler::start(std::size_t num_threads)
	{
		pool_.reserve(num_threads);
		while (num_threads-- != 0) {
			pool_.emplace_back([this]() {
				ctx_.run();
			});
		}
	}

	void scheduler::cancel()
	{
		//
		//	remove work set
		//
		work_.reset();
		ctx_.stop();
		std::this_thread::yield();
	}

	void scheduler::stop()
	{

		//
		//	blocks until all work has finished and there are no more handlers to be dispatched
		//

		std::size_t count{ 0 };
		do 	{
			count = ctx_.run();

#ifdef _DEBUG_TASK
			std::cout 
				<< count
				<< " pending handlers" 
				<< std::endl;
#endif

			std::this_thread::sleep_for(std::chrono::seconds(1));

		}	while (count != 0);

		join();

	}

	void scheduler::join()
	{
		for (auto& t : pool_) {
			t.join();
		}

		pool_.clear();
	}
}
