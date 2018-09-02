/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-async-006.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/async/mux.h>
#include <cyng/io/io_chrono.hpp>
#include <iomanip>
#include <atomic>
#include <fstream>

//	unit_test --run_test=ASYNC/async_006

namespace cyng 
{
	void overtake(dispatcher_t& s1, dispatcher_t& s2, char c)
	{
		std::cerr << "\t" << c << std::endl;
		s1.post([&s2, c]() {
			std::cerr << "\t\t" << c << std::endl;
			std::this_thread::yield();
			s2.post([c]{
				std::cerr << "\t\t\t" << c << std::endl;
			});
		});		
	}
	
	bool test_async_006()
	{
		//
		//	thread pool - 4 threads
		//
		async::scheduler ctx(4);

		dispatcher_t s1(ctx.get_io_service());
		dispatcher_t s2(ctx.get_io_service());

		for (auto c = 'a'; c < 'z'; ++c) {
			overtake(s1, s2, c);
			std::this_thread::yield();
		}
		
		ctx.stop();
		return true;
	}
}
