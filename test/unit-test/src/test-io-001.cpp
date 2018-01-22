
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
		
		auto o_null = factory<null>::create_object();
		type null_t(o_null.get_class());
		std::cout << o_null.get_class().type_name() << std::endl;
		io::serialize_typed(std::cout, o_null);
		std::cout << std::endl;

 		auto o_i = make_object(23);
		type i_t(o_i.get_class());
		std::cout << o_i.get_class().type_name() << std::endl;
		io::serialize_plain(std::cout, o_i);
		std::cout << ", ";
		io::serialize_typed(std::cout, o_i);
		std::cout << std::endl;
		
		auto o_s = make_object("object");
		io::serialize_plain(std::cout, o_s);
		io::serialize_json(std::cout, o_s);
		std::cout << std::endl;

		return true;
	}
	
}
