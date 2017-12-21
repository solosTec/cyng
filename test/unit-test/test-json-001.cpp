
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-json-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/json.h>
#include <cyng/io/serializer.h>
#include <cyng/object.h>

namespace cyng 
{

	bool test_json_001()
	{
		object obj = json::read(std::string(u8"{ \"id\":5, \"subscription\" : \"/data:localhost:26862\", \"λάμδα\" : true, \"channel\" : \"/meta/subscribe\", \"tag\": \"aee65c3b-080f-41be-b864-ec5c5702c477\" }"));
		io::serialize_plain(std::cout, obj);
		std::cout << std::endl;
		io::serialize_json(std::cout, obj);
		std::cout << std::endl;
		
		obj = json::read("{\"cmd\":\"subscribe\",\"channel\":\"sys.cpu.load\",\"timer\":false}");
		io::serialize_json(std::cout, obj);
		std::cout << std::endl;
		std::cout << "{\"cmd\":\"subscribe\",\"channel\":\"sys.cpu.load\",\"timer\":false}" << std::endl;
		return true;
	}
	
}
