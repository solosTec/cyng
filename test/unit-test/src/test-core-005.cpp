/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-core-005.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/tuple_cast.hpp>
#include <cyng/factory.h>
#include <cyng/vm/manip.h>


namespace cyng 
{
	bool test_core_005()
	{
		vector_t vec;
		vec
			<< 1
			<< "hello"
			;
		auto tpl = tuple_cast<int, std::string>(vec);
		BOOST_CHECK_EQUAL(std::get<0>(tpl), 1);
		BOOST_CHECK_EQUAL(std::get<1>(tpl), "hello");
		//std::cout
		//	<< std::get<0>(tpl)
		//	<< ", "
		//	<< std::get<1>(tpl)
		//	;
		return true;
	}
}
