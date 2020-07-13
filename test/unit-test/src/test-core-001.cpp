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
// 		i
// 		ar4 - is array: false
// 		ar4 - tag: 13
// 		N4cyng4nullE
// 		null - tag: 0
// 		N4cyng3eodE
// 		eod - tag: 58 = eod
// 		2
// 		0
// 		St4pairIjNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEE
// 		42, 42
// 		23
// 		
		
		core::wrapper<int> w(1);
		BOOST_CHECK_EQUAL(traits::get_type_name(w.get_class().tag()), "i32");
		
		using array_4 = int[4];
		core::wrapper<array_4>();
		auto ar4 = make_object<array_4>({11, 22, 33, 44});
// 		BOOST_CHECK(ar4.get_class().is_array());	//	This test failes!
				
		auto o_null = factory<null>::create_object();
		BOOST_CHECK_EQUAL(o_null.get_class().tag(), TC_NULL);
		
		auto o_eod = factory<eod>::create_object();
		BOOST_CHECK_EQUAL(traits::get_type_name(o_eod.get_class().tag()), "eod");
		BOOST_CHECK_EQUAL(o_eod.get_class().tag(), TC_EOD);
		
		auto obj = factory<int>::create_object(1);
		tracker t(obj);
		auto o2 = obj;
		BOOST_CHECK_EQUAL( t.use_count(), 2);
		obj.clear();
		o2.clear();
		BOOST_CHECK_EQUAL( t.use_count(), 0);
		
		using pair_t = std::pair<std::size_t, std::string>;
		
		auto o3 = factory<pair_t>::create_object<std::size_t, std::string>(1, "hello");
		
		auto o4 = make_object<int>(42);
		const int n = 0;
		BOOST_CHECK_EQUAL(42, value_cast<int>(o4, n) );
		BOOST_CHECK_NE(nullptr, object_cast<int>(o4));
		BOOST_CHECK_EQUAL(42, *object_cast<int>(o4));
		
		auto o5 = make_object<std::string>("42");
		BOOST_CHECK_EQUAL(*object_cast<std::string>(o5), "42");
		
// 		int array[3] = {1, 2, 3};
		
 		auto o7 = make_object(23);
// 		std::cout << value_cast<int>(o7, 0) << std::endl;
		BOOST_CHECK_EQUAL(23, value_cast<int>(o7, 0));

		auto o8 = factory<int>::create_object<int>(1);
		BOOST_CHECK_EQUAL(1, value_cast<int>(o8, 0));
		
		auto green = make_object<color_8>(0, 255, 0);

		auto o9 = make_object({ 1,2,3 });
		BOOST_CHECK_EQUAL(o9.get_class().tag(), TC_VECTOR);

		auto o10 = make_object(std::vector<std::string>{ "one", "two", "three" });
		BOOST_CHECK_EQUAL(o10.get_class().tag(), TC_VECTOR);

		auto o11 = make_object(make_buffer({1,2,3}));
		BOOST_CHECK_EQUAL(o11.get_class().tag(), TC_BUFFER);

		return true;
	}	
}
