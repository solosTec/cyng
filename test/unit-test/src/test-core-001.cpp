/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-core-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/core/wrapper.hpp>
#include <cyng/factory.h>
#include <cyng/tracker.h>
#include <cyng/object_cast.hpp>
#include <cyng/value_cast.hpp>
#include <cyng/chrono.h>
#include <cyng/intrinsics/color.hpp>

namespace cyng 
{
	bool test_core_001()
	{
		core::wrapper<int> w(1);
		std::cout << w.get_class().type().name() << std::endl;
		
		using array_4 = int[4];
		core::wrapper<array_4>();
		auto ar4 = make_object<array_4>({11, 22, 33, 44});
		std::cout << "ar4 - is array: " << std::boolalpha << ar4.get_class().is_array() << std::endl;
		std::cout << "ar4 - tag: " << ar4.get_class().tag() << std::endl;
				
		auto o_null = factory<null>::create_object();
		std::cout << o_null.get_class().type().name() << std::endl;
		std::cout << "null - tag: " << o_null.get_class().tag() << std::endl;
		
		auto o_eod = factory<eod>::create_object();
		std::cout << o_eod.get_class().type().name() << std::endl;
		std::cout << "eod - tag: " << o_eod.get_class().tag() << " = " << traits::get_type_name(o_eod.get_class().tag()) << std::endl;

		auto obj = factory<int>::create_object(1);
		tracker t(obj);
		auto o2 = obj;
		std::cout << t.use_count() << std::endl;
		obj.clear();
		o2.clear();
		std::cout << t.use_count() << std::endl;
		
		using pair_t = std::pair<std::size_t, std::string>;
		
		auto o3 = factory<pair_t>::create_object<std::size_t, std::string>(1, "hello");
		std::cout << o3.get_class().type().name() << std::endl;
		
		auto o4 = make_object<int>(42);
		const int n = 0;
		std::cout << value_cast<int>(o4, n) 
			<< ", " 
			<< *object_cast<int>(o4) 
			<< std::endl;

		auto o5 = make_object<std::string>("42");
// 		std::cout << "o5=" << *object_cast<std::string>(o5) << std::endl;
		BOOST_CHECK_EQUAL(*object_cast<std::string>(o5), "42");
		
// 		int array[3] = {1, 2, 3};
		
 		auto o7 = make_object(23);
		std::cout << value_cast<int>(o7, 0) << std::endl;

		auto o8 = factory<int>::create_object<int>(1);
		
		auto green = make_object<color_8>(0, 255, 0);

		auto o9 = make_object({ 1,2,3 });

		
		return true;
	}	
}
