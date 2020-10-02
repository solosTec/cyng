
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
#include <cyng/json/json_inc_parser.h>

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
        
		auto const inp_01 = std::string("{ \"id\":5, \"subscription\" : \"/data:localhost:26862\", \"λάμδα\" : true, \"channel\" : \"/meta/subscribe\", \"tag\": \"aee65c3b-080f-41be-b864-ec5c5702c477\" }");
		object obj = json::read(inp_01);
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
		//object obj = json::read(std::string("{ \"id\":5, \"subscription\" : \"/data:localhost:26862\", \"λάμδα\" : true, \"channel\" : \"/meta/subscribe\", \"tag\": \"aee65c3b-080f-41be-b864-ec5c5702c477\" }"));

		json::parser parser([&ss](vector_t&& vec) {
			//std::cout << cyng::io::to_type(vec) << std::endl;
            ss.str("");
            io::serialize_json(ss, vec.at(0));
            std::cout << ss.str() << std::endl;
            });

		auto const s1 = std::string(R"(
{"menu": {
    "header": "SVG Viewer",
    "items": [
        {"id": "Open"},
        {"id": "OpenNew", "label": "Open New"},
        null,
        true,
        false,
        {"id": "ZoomIn", "label": "Zoom In"},
        {"id": "ZoomOut", "label": "Zoom Out"},
        {"id": "OriginalView", "label": "Original View"},
        null,
        {"id": "Quality"},
        {"id": "Pause"},
        {"id": "Mute"},
        null,
        {"id": "Find", "label": "Find..."},
        {"id": "FindAgain", "label": "Find Again"},
        {"id": "Copy"},
        {"id": "CopyAgain", "label": "Copy Again"},
        {"id": "CopySVG", "label": "Copy SVG"},
        {"id": "ViewSVG", "label": "View SVG"},
        {"id": "ViewSource", "label": "View Source"},
        {"id": "SaveAs", "label": "Save As"},
        null,
        {"id": "Help"},
        {"id": "About", "label": "About Adobe CVG Viewer..."}
    ]
}}
		)");

		//parser.read(std::begin(s1), std::end(s1));

        //  
        //  the UTF-8 sequence from key4 results in
        //  z\u00df\u6c34\u1f34c or
        //  0x7a 0xc3, 0x9f 0xe6 0xb0, 0xb4 0xf0 0x9f 0x8d 0x8c
        //  
        auto const s2 = std::string(R"(
        {
            "key-0": [12,22.0,3e-10,[4, 5],[],6],
            "key-1": "value\t1",
            "key-2": "value\u2514",
            "key-3": [
                { "key4": "zß水🍌" },
                null,
                true,
                { "key5a": "value5a", "key5b": "value5b"}
            ],
            "key-6": false,
            "key-7": [1,2,3,4, 5],
            "key-8": "eof"
        }
		)");

        parser.read(std::begin(s2), std::end(s2));

        //parser.read(std::begin(inp_01), std::end(inp_01));
        return true;
	}
	
}
