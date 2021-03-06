/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-core-004.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/chrono.h>
#include <cyng/traits.h>
#include <cyng/sys/cpu.h>
#include <thread>

namespace cyng 
{
	bool test_core_004()
	{
		std::cout << chrono::to_string(std::chrono::system_clock::now()) << "---" << std::endl;
// 		
// 		auto tp1 = make_object(std::chrono::system_clock::now());
// 		tp1.serialize(std::cout) << std::endl;
// 		
//         auto tp2 = make_object(std::chrono::system_clock::from_time_t(chrono::tm_to_tt(chrono::init_tm(2001, 8, 19, 2, 3, 4))));
// 		tp2.serialize(std::cout) << std::endl;
// 
		std::cout << "eod idx: " << type_tag_traits<eod>() << std::endl;
		std::cout << "buffer_t idx: " << type_tag_traits<buffer_t>() << std::endl;
		std::cout << "long double idx: " << type_tag_traits<long double>() << std::endl;
		
		std::cout << "buffer_t built-in: " << std::boolalpha << built_in_type<buffer_t>() << std::endl;
		std::cout << "long double built-in: " << std::boolalpha << built_in_type<long double>() << std::endl;
		
// 		auto u1 = make_object(boost::uuids::random_generator()());
// 		std::cout << "uuid tag: " << u1.get_class().tag() <<  " = " << traits::get_tag_name(u1.get_class().tag()) << std::endl;
		for (std::size_t idx = 0; idx < 10; ++idx)
		{
			std::cout << sys::get_total_cpu_load() << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
// 		std::cout << sys::get_total_cpu_load() << std::endl;
// 		
		return true;
	}
}
