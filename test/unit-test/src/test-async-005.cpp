/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-async-005.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/async/mux.h>
#include <cyng/io/io_chrono.hpp>
#include <iomanip>
#include <atomic>
#include <fstream>

//	unit_test --run_test=ASYNC/async_005

namespace cyng 
{

	bool test_async_005()
	{
		//
		//	thread pool - 4 threads
		//
		async::scheduler ctx(4);

		std::vector<dispatcher_t> dispatchers;
		for (std::size_t pos = 0; pos < 100; pos++)
		{
			dispatchers.emplace_back(ctx.get_io_service());
		}

		std::size_t counter{ 0 };
		std::for_each(dispatchers.begin(), dispatchers.end(), [&counter](dispatcher_t& strand) {
			++counter;
			std::cerr << "post: " << counter << std::endl;

			strand.post([counter]() {

				std::cerr << "... thread: " << counter << " / " << std::this_thread::get_id() << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cerr << "thread: " << counter << " / " << std::this_thread::get_id() << " ..." << std::endl;
			});

		});

		ctx.stop();
		return true;
	}
}
