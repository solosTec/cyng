/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-async-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/async/task/task_builder.hpp>
#include <cyng/factory/set_factory.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/random_generator.hpp>

namespace cyng 
{
	
	class simple
	{
	public:
		using msg_0 = std::tuple<int, std::string>;
		using msg_1 = std::tuple<boost::uuids::uuid>;
		using signatures_t = std::tuple<msg_0, msg_1>;
		
	public:
		simple(async::base_task* bt, int i, std::string name)
		: base_(bt)
		, counter_(0)
		{
			std::cout << "constructor simple: " << i << ", " << name << std::endl;
		}
		
		//	slot 0
		continuation process(int i, std::string name)
		{
			std::cout << "simple::slot-0($" << base_->get_id() << ", " << i  << ", " << name << ")" << std::endl;
			return continuation::TASK_CONTINUE;
		}
		
		//	slot 1
		continuation process(boost::uuids::uuid tag)
		{
			std::cout << "simple::slot-1($" << base_->get_id() << ", " << tag  << ")" << std::endl;
			return continuation::TASK_CONTINUE;
		}
		
		continuation run()
		{
			counter_++;
			std::cout << "simple::run(" << base_->get_id() << ", #" << counter_ << ")" << std::endl;
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
			std::cout << "simple::stop(" << base_->get_id() << ")" << std::endl;			
		}

	private:
		async::base_task* base_;
		std::size_t counter_;
//		bool shutdown_;
	};

	//
	//	initialize static slot names
	//
	template <>
	std::map<std::string, std::size_t> async::task<simple>::slot_names_({ {"slot-0", 0} });

	bool test_async_002()
	{
	
		async::mux task_manager;
		
	
		for (int idx = 0; idx < 100; )
		{
			async::start_task_delayed<simple>(task_manager, std::chrono::seconds(idx), idx++, "welcome-" + std::to_string(idx));
			async::start_task_sync<simple>(task_manager, idx++, "welcome-" + std::to_string(idx));
		}
		
		std::cout << "wait..." << std::endl;
		task_manager.post(2, 0, tuple_factory(100, std::string("event-1")));
		task_manager.post(3, 0, tuple_factory(101, "event-3"));

		//
		//	slot by name
		//
		task_manager.post(4, "slot-0", tuple_factory(102, "event-4"));

		//
		//	compiler selects slot
		//
#if !BOOST_COMP_GNUC
		//	doesn't compile with g++
		task_manager.send<simple, 0ul>(tuple_factory(200, "event-5"));
#endif
		
		for (int idx = 0; idx < 10; idx++)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			task_manager.post(idx, 1, tuple_factory(boost::uuids::random_generator()()));
		}
		
		std::this_thread::sleep_for(std::chrono::seconds(20));
		std::cout << "stop task manager..." << std::endl;	
		task_manager.stop();
// 		std::this_thread::sleep_for(std::chrono::seconds(2));
		std::cout << "stop I/O service..." << std::endl;	
		task_manager.get_io_service().stop();
		
		return true;
	}
	
}
