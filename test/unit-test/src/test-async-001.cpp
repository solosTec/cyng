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
#if defined(_CYNG_CPP_SUPPORT_N4508)
		std::cout << "std::shared_mutex support" << std::endl;
#else 
		std::cout << "no std::shared_mutex supportF" << std::endl;

#endif    
		using tpl = std::tuple<int, char, long, std::string>;
		std::cout << meta::size<tpl>::value << std::endl;
		BOOST_CHECK_EQUAL(meta::size<tpl>::value, std::tuple_size<tpl>::value);
// 		
#if defined(_CYNG_CPP_SUPPORT_N3915)
		std::cout << meta::apply(add, std::make_pair(1, 2)) << '\n';
#else
		//	support required to get the right return value type
		std::cout << meta::apply(add, std::make_pair(1, 2)) << '\n';
#endif

		using target = std::tuple<int, char, long, std::string>;
		auto inp2 = create_inp();
		
 		using cf_t = meta::converter_factory<target>;
		target t = cf_t::copy(std::begin(inp2), std::end(inp2));
		std::cout << std::get<0>(t) << '\n';
		std::cout << std::get<1>(t) << '\n';
		std::cout << std::get<2>(t) << '\n';
		std::cout << std::get<3>(t) << '\n';
		
		
		tuple_t lst = tuple_factory(1234, std::string("4321"));
		for (auto e : lst)
		{
			std::cout << "lst[" << lst.size() << "] = ";
			io::serialize_plain(std::cout, e);
// 			e.serialize(std::cout);
			std::cout << std::endl;
		}
// 		
		lst = tuple_factory(std::make_tuple(1, "WoW", std::chrono::minutes(7)));
		for (auto e : lst)
		{
			std::cout << "lst[" << lst.size() << "] = ";
			io::serialize_plain(std::cout, e);
			std::cout << std::endl;
		}
// 		
// 		
// 		vector_factory(std::vector({1, 2, 3}));
		
		return true;
	}
	
}
