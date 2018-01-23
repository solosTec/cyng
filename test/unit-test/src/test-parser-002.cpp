
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-parser-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/parser/object_parser.h>
#include <cyng/io/serializer.h>
#include <cyng/factory.h>
#include <cyng/io/io_chrono.hpp>

namespace cyng 
{

	std::pair<object, bool > parse_object(std::string const& inp)
	{
		object result;
		
		auto first(begin(inp)), last(end(inp));
		utf::convert_u8_to_u32 f(first), l(last);
		static const object_parser<utf::convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> g;
		const bool r = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::qi::standard_wide::space, result);
		
		return (r)
			? std::make_pair(result, r)
			: std::make_pair(make_object(), r)
			;
	}

	
	bool test_parser_002()
	{
		//std::pair<std::chrono::system_clock::time_point, bool > r = parse_db_timestamp("2017-11-26 19:14:42");
		std::pair<std::chrono::system_clock::time_point, bool > r = parse_db_timestamp("30.09.2016 13:34:26");
		
		//io::serialize_typed(std::cout, r.first);
		std::cout << cyng::to_str(r.first);

		//std::pair<object, bool > r1 = parse_object(u8"20chrono:sec");
 		std::pair<object, bool > r1 = parse_object(u8"\"30.09.2016 08:29:09\"chrono:tp");
		BOOST_CHECK(r1.second);
// 		2017-11-26 19:14:42.00000000
		io::serialize_typed(std::cout, r1.first);

		std::pair<object, bool > r2 = parse_object("42.2");
		BOOST_CHECK(r2.second);
		io::serialize_typed(std::cout, r2.first);
		
		
		return true;
	}
	
}
