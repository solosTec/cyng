/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-core-003.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <cyng/factory.h>
#include <cyng/type.h>
#include <cyng/io/serializer.h>

namespace cyng 
{
	bool test_core_003()
	{
// 		a1: 1
// 		a1: i32
// 		a1: 2
// 		a2: 0
// 		a2: v
// 		a2: 1
// 		a2: 20003
// 		v2.3
// 		b1: truebool
// 		b2: falsebool
		
		auto o1 = make_object(23);
		
		auto a1 = attr_factory(1, 2);
		type t1(a1.second.get_class());
		BOOST_CHECK(t1.is_same<int>());
// 		std::cout << "a1: " << t1.is_same<int>() << std::endl;
		BOOST_CHECK_EQUAL(a1.second.get_class().type_name(), "i32");
//   		std::cout << "a1: " << a1.second.get_class().type_name() << std::endl;
//   		std::cout << "a1: " << std::dec << value_cast<int>(a1.second, 0) << std::endl;
		BOOST_CHECK_EQUAL(value_cast<int>(a1.second, 0), 2);
		
		auto a2 = attr_factory<version>(1, { 2, 3});
		type t2(a2.second.get_class());
		BOOST_CHECK(!t2.is_same<int>());
// 		std::cout << "a2: " << t2.is_same<int>() << std::endl;
//   		std::cout << "a2: " << a2.second.get_class().type_name() << std::endl;
		BOOST_CHECK_EQUAL(a2.first, 1);
//   		std::cout << "a2: " << a2.first << std::endl;
//   		std::cout << "a2: " << std::hex << object_cast<version>(a2.second)->full() << std::endl;
		BOOST_CHECK_NE(nullptr, object_cast<version>(a2.second));
		BOOST_CHECK_EQUAL(object_cast<version>(a2.second)->full(), 0x20003);
// 		io::serialize_typed(std::cout, a2.second);
// 		std::cout<< std::endl;
 
// 
  		auto const a3 = param_factory<version>("name", { 4, 5 });
  		type const t3(a3.second.get_class());
//  		std::cout << "a3: " << t3.is_same<int>() << std::endl;
//    		std::cout << "a3: " << t3.get_name() << std::endl;
//   		std::cout << "a3: " << a3.first << std::endl;
//    		std::cout << "a3: " << std::hex << object_cast<version>(a3.second)->full() << std::endl;
// 		std::cout << "a3: " << std::boolalpha << a3.second.get_class().is_enum() << std::endl;
		BOOST_CHECK_NE(nullptr, object_cast<version>(a3.second));
		BOOST_CHECK_EQUAL(object_cast<version>(a3.second)->full(), 0x4005);
// 		
// 		auto op1 = make_object(code::NOOP);
// 		std::cout << "op-1: " << std::boolalpha << op1.get_class().is_enum() << std::endl;
// 		value_cast<code>(op1);
// 		
//#if defined(CYNG_LEGACY_MODE_ON)
//        long unsigned int idx{1ul};
//		auto a4 = make_object(std::make_pair(idx, "one"));
//#endif
// 		//	doesn't create an attribute for obvious reasons
// // 		auto a5 = make_object(std::make_pair(2, "two"));
// 		auto p1 = make_object(std::make_pair(std::string("one"), 1));
// 		auto p2 = make_object(std::make_pair("one", 1));
// 		
		auto b1 = make_object(true);
		auto b2 = make_object(false);
// 		std::cout << "b1: ";
		std::stringstream ss;
		io::serialize_typed(ss, b1);
// 		std::cout<< std::endl;
// 		std::cout << "b2: ";
		ss.str("");
		io::serialize_typed(ss, b2);
// 		std::cout<< std::endl;
		
		
		return true;
	}
}
