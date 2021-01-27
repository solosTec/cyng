
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-log-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/log.h>
#include <cyng/async/scheduler.h>

namespace cyng 
{

	bool test_log_001()
	{
		
		async::scheduler ios;
// 		logging::console_logger_t clog(ios.get_io_service(), "test");
		auto clog = logging::make_console_logger(ios.get_io_service(), "test");
		
		CYNG_LOG_INFO(clog, "hello," << " world!");
		
		std::this_thread::sleep_for(std::chrono::seconds(1));
		CYNG_LOG_WARNING(clog, "low fuel");

		std::this_thread::sleep_for(std::chrono::seconds(1));
		CYNG_LOG_ERROR(clog, "you are attacked");

		std::this_thread::sleep_for(std::chrono::seconds(1));
		CYNG_LOG_TRACE(clog, "heading" << " " << "north");

		std::this_thread::sleep_for(std::chrono::seconds(2));
		
// 		logging::file_logger_t flog(ios.get_io_service(), "demo.log");
		auto flog = logging::make_file_logger(ios.get_io_service(), "demo.log");
		for (auto idx = 0; idx < 200000; ++idx)
		{
			ios.invoke([idx, flog](){
				CYNG_LOG_TRACE(flog, "entry " << idx);
			});
		}
		
		std::this_thread::sleep_for(std::chrono::seconds(5));
// 		ios.stop();
		return true;
	}
	
}
