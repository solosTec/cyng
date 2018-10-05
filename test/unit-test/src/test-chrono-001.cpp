/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-chrono-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/chrono.h>
#include <cyng/io/io_chrono.hpp>

namespace cyng 
{
	bool test_chrono_001()
	{
		//{
		//	//
		//	//	iterate over month
		//	//
		//	auto ts = std::chrono::system_clock::now();
		//	for (auto idx = 0; idx < 24; ++idx) {

		//		//std::cout
		//		//	<< to_str(ts)
		//		//	<< std::endl
		//		//	;

		//		//	add one month
		//		ts = chrono::add_month(ts, 1);
		//	}
		//}
		{
			//2018-10-31 11:44:21 has 61 days
			//2019-01-29 11:44:21 has 59 days
			//2019-01-30 11:44:21 has 59 days
			//2019-01-31 11:44:21 has 59 days

			auto tp = chrono::init_tp(2018, 10, 31, 11, 44, 21);
			auto r = chrono::days_of_month(tp);
			BOOST_CHECK(r.count() == 31);
		}

		{
			auto ts = std::chrono::system_clock::now();
			for (auto idx = 0; idx < 365; ++idx) {

				auto r = chrono::days_of_month(ts);
				//std::cout
				//	<< to_str(ts)
				//	<< " has "
				//	<< r.count()
				//	<< " days"
				//	<< std::endl
				//	;
				BOOST_CHECK(r.count() < 32);
				BOOST_CHECK(r.count() > 26);
				ts += std::chrono::hours(24);
			}
		}
		return true;
	}	
}
