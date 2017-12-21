/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-core-003.h"
#include <boost/test/unit_test.hpp>
#include <iostream>
// #include <cyng/core/wrapper.hpp>
#include <cyng/factory.h>
// #include <cyng/tracker.h>
// #include <cyng/object_cast.hpp>
// #include <cyng/value_cast.hpp>
#include <cyng/type.h>
// #include <cyng/chrono.h>
#include <cyng/io/serializer.h>
// #include <cyng/traits.h>
// #include <boost/uuid/uuid_generators.hpp>

namespace cyng 
{
	bool test_core_003()
	{
		auto o1 = make_object(23);
		
		auto a1 = attr_factory(1, 2);
		type t1(a1.second.get_class());
		std::cout << "a1: " << t1.is_same<int>() << std::endl;
  		std::cout << "a1: " << a1.second.get_class().type_name() << std::endl;
  		std::cout << "a1: " << std::dec << value_cast<int>(a1.second, 0) << std::endl;

		auto a2 = attr_factory<version>(1, { 2, 3});
		type t2(a2.second.get_class());
		std::cout << "a2: " << t2.is_same<int>() << std::endl;
  		std::cout << "a2: " << a2.second.get_class().type_name() << std::endl;
  		std::cout << "a2: " << a2.first << std::endl;
  		std::cout << "a2: " << std::hex << object_cast<version>(a2.second)->full() << std::endl;
		io::serialize_formatted(std::cout, a2.second);
		std::cout<< std::endl;
 
// 
//  		auto a3 = param_factory<version>("name", { 4, 5 });
//  		type t3(a3.second.get_class());
//  		std::cout << "a3: " << t3.is_same<int>() << std::endl;
//    		std::cout << "a3: " << t3.get_name() << std::endl;
//   		std::cout << "a3: " << a3.first << std::endl;
//   		std::cout << "a3: " << std::hex << object_cast<version>(a3.second)->full() << std::endl;
// 		std::cout << "a3: " << std::boolalpha << a3.second.get_class().is_enum() << std::endl;
// 		
// 		auto op1 = make_object(code::NOOP);
// 		std::cout << "op-1: " << std::boolalpha << op1.get_class().is_enum() << std::endl;
// 		value_cast<code>(op1);
// 		
		auto a4 = make_object(std::make_pair(1ul, "one"));
// 		//	doesn't create an attribute for obvious reasons
// // 		auto a5 = make_object(std::make_pair(2, "two"));
// 		auto p1 = make_object(std::make_pair(std::string("one"), 1));
// 		auto p2 = make_object(std::make_pair("one", 1));
// 		
		auto b1 = make_object(true);
		auto b2 = make_object(false);
		std::cout << "b1: ";
		io::serialize_formatted(std::cout, b1);
		std::cout<< std::endl;
		std::cout << "b2: ";
		io::serialize_formatted(std::cout, b2);
		std::cout<< std::endl;
		
		
		return true;
	}
}
