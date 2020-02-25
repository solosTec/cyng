
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-io-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/factory.h>
#include <cyng/type.h>
#include <cyng/io/serializer.h>

namespace cyng 
{

	bool test_io_001()
	{
// 		null
// 		nullnull
// 		i32
// 		23, 23i32
// 		object"object"
		
		auto o_null = factory<null>::create_object();
		type null_t(o_null.get_class());
		BOOST_CHECK_EQUAL(o_null.get_class().type_name(), "null");
// 		io::serialize_typed(std::cout, o_null);	//	nullnull

		std::stringstream ss;
 		auto o_i = make_object(23);
		type i_t(o_i.get_class());
		BOOST_CHECK_EQUAL(o_i.get_class().type_name(), "i32");
		io::serialize_plain(ss, o_i);
		BOOST_CHECK_EQUAL(ss.str(), "23");
		ss.str("");
		io::serialize_typed(ss, o_i);
		BOOST_CHECK_EQUAL(ss.str(), "23i32");
		
		auto o_s = make_object("object");
		ss.str("");
		io::serialize_plain(ss, o_s);
		BOOST_CHECK_EQUAL(ss.str(), "object");
		ss.str("");
		io::serialize_json(ss, o_s);
		BOOST_CHECK_EQUAL(ss.str(), "\"object\"");

		return true;
	}
	
}
