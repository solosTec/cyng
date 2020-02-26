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
    static std::size_t stop_counter{0};
	
	class task_002
	{
	public:
		using msg_0 = std::tuple<int, std::string>;
		using msg_1 = std::tuple<boost::uuids::uuid>;
		using signatures_t = std::tuple<msg_0, msg_1>;
		
	public:
		task_002(async::base_task* bt, int i, std::string name)
		: base_(bt)
		, counter_(0)
		{
// 			std::cout << "constructor task_002: " << i << ", " << name << std::endl;
		}
		
		//	slot 0
		continuation process(int i, std::string name)
		{
//          task_002::slot-0($2, 100, event-1)
//          task_002::slot-0($3, 101, event-3)
//          task_002::slot-0($4, 102, event-4)
            
// 			std::cout << "task_002::slot-0($" << base_->get_id() << ", " << i  << ", " << name << ")" << std::endl;
            switch(base_->get_id()) {
                case 2:
                    BOOST_CHECK_EQUAL(i, 100);
                    BOOST_CHECK_EQUAL(name, "event-1");
                    break;
                case 3:
                    BOOST_CHECK_EQUAL(i, 101);
                    BOOST_CHECK_EQUAL(name, "event-3");
                    break;
                case 4:
                    BOOST_CHECK_EQUAL(i, 102);
                    BOOST_CHECK_EQUAL(name, "event-4");
                    break;
                default:
                    break;
            }
			return continuation::TASK_CONTINUE;
		}
		
		//	slot 1
		continuation process(boost::uuids::uuid tag)
		{
// 			std::cout << "task_002::slot-1($" << base_->get_id() << ", " << tag  << ")" << std::endl;
			return continuation::TASK_CONTINUE;
		}
		
		continuation run()
		{
			counter_++;
// 			std::cout << "task_002::run(" << base_->get_id() << ", #" << counter_ << ")" << std::endl;
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
            BOOST_CHECK_EQUAL(stop_counter, base_->get_id());
            --stop_counter;
// 			std::cout << "task_002::stop(" << base_->get_id() << ")" << std::endl;			
		}

	private:
		async::base_task* base_;
		std::size_t counter_;
	};

	//
	//	initialize static slot names
	//
	template <>
	std::map<std::string, std::size_t> async::task<task_002>::slot_names_({ {"slot-0", 0}, {"slot-1", 1} });

	bool test_async_002()
	{
	
		//
		//	task multiplexer
		//
		async::mux task_manager;
			
		//
		//	start 100 + 1 tasks
		//
		for (int idx = 0; idx < 100; )
		{
			++idx;
			async::start_task_delayed<task_002>(task_manager, std::chrono::seconds(idx), idx, "welcome-" + std::to_string(idx));
			++idx;
			async::start_task_sync<task_002>(task_manager, idx, "welcome-" + std::to_string(idx));
		}
		
// 		std::cout << "wait..." << std::endl;
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
		task_manager.send<task_002, 0ul>(tuple_factory(200, "event-5"));
#endif
		
		for (int idx = 0; idx < 10; idx++)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			task_manager.post(idx, 1, tuple_factory(boost::uuids::random_generator()()));
		}
		
		std::this_thread::sleep_for(std::chrono::seconds(20));
// 		std::cout << "stop task manager..." << std::endl;
        stop_counter = 100u;
		task_manager.stop(std::chrono::seconds(2), 2);
// 		std::cout << "stop I/O service..." << std::endl;	
		task_manager.get_io_service().stop();
        BOOST_CHECK(task_manager.get_io_service().stopped());
		
		return true;
	}
	
}
