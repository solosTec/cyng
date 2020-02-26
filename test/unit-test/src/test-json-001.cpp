
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
//     {("id":5),("subscription":/data:localhost:26862),("λάμδα":true),("channel":/meta/subscribe),("tag":aee65c3b-080f-41be-b864-ec5c5702c477)}
//     {"id": 5, "subscription": "/data:localhost:26862", "λάμδα": true, "channel": "/meta/subscribe", "tag": "aee65c3b-080f-41be-b864-ec5c5702c477"}
//     {"cmd": "subscribe", "channel": "sys.cpu.load", "timer": false}
//     {"cmd":"subscribe","channel":"sys.cpu.load","timer":false}
        
		object obj = json::read(std::string("{ \"id\":5, \"subscription\" : \"/data:localhost:26862\", \"λάμδα\" : true, \"channel\" : \"/meta/subscribe\", \"tag\": \"aee65c3b-080f-41be-b864-ec5c5702c477\" }"));
        std::stringstream ss;
		io::serialize_plain(ss, obj);
// 		std::cout << std::endl;
        BOOST_CHECK_EQUAL(ss.str(), "{(\"id\":5),(\"subscription\":/data:localhost:26862),(\"λάμδα\":true),(\"channel\":/meta/subscribe),(\"tag\":aee65c3b-080f-41be-b864-ec5c5702c477)}");
        
        ss.str("");
		io::serialize_json(ss, obj);
// 		std::cout << std::endl;
        BOOST_CHECK_EQUAL(ss.str(), "{\"id\": 5, \"subscription\": \"/data:localhost:26862\", \"λάμδα\": true, \"channel\": \"/meta/subscribe\", \"tag\": \"aee65c3b-080f-41be-b864-ec5c5702c477\"}");
		
		obj = json::read("{\"cmd\":\"subscribe\",\"channel\":\"sys.cpu.load\",\"timer\":false}");
        ss.str("");
		io::serialize_json(ss, obj);
        BOOST_CHECK_EQUAL(ss.str(), "{\"cmd\": \"subscribe\", \"channel\": \"sys.cpu.load\", \"timer\": false}");
// 		std::cout << std::endl;
// 		std::cout << "{\"cmd\":\"subscribe\",\"channel\":\"sys.cpu.load\",\"timer\":false}" << std::endl;
		return true;
	}
	
}
