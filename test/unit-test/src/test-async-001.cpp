/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-async-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/meta.h>
#include <cyng/factory.h>
#include <cyng/io/io_chrono.hpp>
#include <cyng/io/serializer.h>

namespace cyng 
{
	int add(int first, int second) { return first + second; }	

	tuple_t create_inp ()
	{
		return tuple_t{
			make_object(1),
			make_object('a'),
			make_object(2l),
			make_object("hello")
		};
	}

	bool test_async_001()
	{
        
// #if defined(__CPP_SUPPORT_N4508)
// 		std::cout << "std::shared_mutex support" << std::endl;
// #else 
// 		std::cout << "no std::shared_mutex supportF" << std::endl;
// 
// #endif    
        
//     std::shared_mutex support
//     4
//     3
//     1
//     a
//     2
//     hello
//     lst[2] = 1234
//     lst[2] = 4321
//     lst[3] = 1
//     lst[3] = WoW
//     lst[3] = 00:07:0.000000
        
		using tpl = std::tuple<int, char, long, std::string>;
// 		std::cout << meta::size<tpl>::value << std::endl;
		BOOST_CHECK_EQUAL(meta::size<tpl>::value, std::tuple_size<tpl>::value);
// 		
// #if defined(__CPP_SUPPORT_N3915)
// 		std::cout << meta::apply(add, std::make_pair(1, 2)) << '\n';
// #else
// 		//	support required to get the right return value type
// 		std::cout << meta::apply(add, std::make_pair(1, 2)) << '\n';
// #endif
        BOOST_CHECK_EQUAL(3, meta::apply(add, std::make_pair(1, 2)));

		using target = std::tuple<int, char, long, std::string>;
		auto inp2 = create_inp();
		
 		using cf_t = meta::converter_factory<target>;
		target t = cf_t::copy(std::begin(inp2), std::end(inp2));
        BOOST_CHECK_EQUAL(std::get<0>(t), 1);
        BOOST_CHECK_EQUAL(std::get<1>(t), 'a');
        BOOST_CHECK_EQUAL(std::get<2>(t), 2);
        BOOST_CHECK_EQUAL(std::get<3>(t), "hello");
		
//     lst[2] = 1234
//     lst[2] = 4321
		tuple_t lst = tuple_factory(1234, std::string("4321"));
        BOOST_CHECK_EQUAL(lst.size(), 2);
        {
            std::stringstream ss;
			io::serialize_plain(ss, lst.front());
            BOOST_CHECK_EQUAL(ss.str(), "1234");
        }
        {
            std::stringstream ss;
			io::serialize_plain(ss, lst.back());
            BOOST_CHECK_EQUAL(ss.str(), "4321");
        }

        lst = tuple_factory(std::make_tuple(1, "WoW", std::chrono::minutes(7)));
        std::size_t idx{ 0 };
		for (auto e : lst)
		{
	        std::stringstream ss;
            io::serialize_plain(ss, e);
            switch(idx) {
                case 0:
                    BOOST_CHECK_EQUAL(ss.str(), "1");
                    break;
                case 1:
                    BOOST_CHECK_EQUAL(ss.str(), "WoW");
                    break;
                case 2:
                    BOOST_CHECK_EQUAL(ss.str(), "00:07:0.000000");
                    break;
                default:
                    break;
            }
            ++idx;
		}
// 		
// 		
// 		vector_factory(std::vector({1, 2, 3}));
		
		return true;
	}
	
}
