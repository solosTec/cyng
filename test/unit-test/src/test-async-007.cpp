/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#include "test-async-007.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/async/tmux.h>
#include <cyng/async/task/task_builder.hpp>
#include <cyng/factory/set_factory.h>

//	unit_test --run_test=ASYNC/async_007

namespace cyng 
{
	class task_007
	{
	public:
		using msg_0 = std::tuple<int, std::string>;
		using msg_1 = std::tuple<boost::uuids::uuid>;
		using signatures_t = std::tuple<msg_0, msg_1>;

	public:
		task_007(async::base_task* bt, int i, std::string name)
			: base_(bt)
			, counter_(0)
		{
			std::cout << "constructor task_007: " << i << ", " << name << std::endl;
		}

		//	slot 0
		continuation process(int i, std::string name)
		{
			std::cout << "task_007::slot-0($" << base_->get_id() << ", " << i << ", " << name << ")" << std::endl;
			return continuation::TASK_CONTINUE;
		}

		//	slot 1
		continuation process(boost::uuids::uuid tag)
		{
			std::cout << "task_007::slot-1($" << base_->get_id() << ", " << tag << ")" << std::endl;
			return continuation::TASK_CONTINUE;
		}

		continuation run()
		{
			counter_++;
			std::cout << "task_007::run(" << base_->get_id() << (((base_->get_id() % 2) == 0) ? "-" : "\\" ) << ", #" << counter_ << ")" << std::endl;
			if (base_->get_id() > counter_)
			{
				base_->suspend(std::chrono::seconds(base_->get_id()) - std::chrono::seconds(counter_));
			}
			else if ((base_->get_id() * 1000) > counter_)
			{
				base_->suspend(std::chrono::seconds(base_->get_id()) - std::chrono::milliseconds(counter_));
			}
			else
			{
				base_->suspend(std::chrono::milliseconds(counter_));
				// 				base_->suspend(std::chrono::duration<double, std::milli>(1.0/counter_));
			}
			return continuation::TASK_CONTINUE;
		}
		void stop(bool /*shutdown*/)
		{
			std::cout << "task_007::stop(" << base_->get_id() << ")" << std::endl;
		}

	private:
		async::base_task* base_;
		std::size_t counter_;
	};

	bool test_async_007()
	{
		//
		//	task multiplexer
		//
		async::tmux tm;

		//
		//	start 100 + 1 tasks
		//
		for (int idx = 0; idx < 100; )
		{
			++idx;
			async::start_task_delayed<task_007>(tm.get_mux(), std::chrono::seconds(idx), idx, "welcome-" + std::to_string(idx));
			++idx;
			async::start_task_sync<task_007>(tm.get_mux(), idx, "welcome-" + std::to_string(idx));
		}

		std::cout << "wait..." << std::endl;
		tm.get_mux().post(2, 0, tuple_factory(100, std::string("event-1")));
		tm.get_mux().post(3, 0, tuple_factory(101, "event-3"));

		//
		//	stop a specific task
		//
		tm.stop(100u);
		tm.stop(101u);

		std::this_thread::sleep_for(std::chrono::seconds(30));

		std::cout << "stop task manager..." << std::endl;
		tm.get_mux().stop(std::chrono::milliseconds(100), 10);
		std::cout << "stop I/O service..." << std::endl;
		tm.get_mux().get_io_service().stop();

		return true;
	}
}
