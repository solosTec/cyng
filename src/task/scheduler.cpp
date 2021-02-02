#include <cyng/task/scheduler.h>
#include <algorithm>

namespace docscript {

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

	void scheduler::stop()
	{
		//
		//	remove work set
		//
		work_.reset();

		//
		//	blocks until all work has finished and there are no more handlers to be dispatched
		//
		while (ctx_.run() != 0)
		{
			//std::cout << "pending handlers" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}

		ctx_.stop();

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
