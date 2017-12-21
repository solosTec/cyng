
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-parser-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/parser/numeric_parser.h>
#include <cyng/parser/chrono_parser.h>
#include <cyng/parser/utf8_parser.h>
#include <cyng/parser/token_parser.h>
#include <cyng/io/io_chrono.hpp>
#include <cyng/io/serializer.h>
#include <cyng/factory.h>

namespace cyng 
{

	std::pair<object, bool > parse_numeric(std::string const& inp)
	{
		object result;
		numeric_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return (r)
			? std::make_pair(result, r)
			: std::make_pair(make_object(), r)
			;
	}
	
	
	bool test_parser_001()
	{
		/**
		 * parse a SQL timestamp
		 * "2015-11-28 11:06:44" and "30.09.2016 13:34:26"
		 */
		std::pair<std::chrono::system_clock::time_point, bool > r1 = parse_db_timestamp("2017-11-26 19:14:42");
		BOOST_CHECK(r1.second);
// 		2017-11-26 19:14:42.00000000
		std::cout << r1.first << std::endl;
		
		std::pair<object, bool > r2 = parse_numeric("42.2");
		BOOST_CHECK(r2.second);
		io::serialize_plain(std::cout, r2.first);
		std::cout << r2.first.get_class().type().name() << std::endl;
		
		//
		//	test 
		//	* u8_to_u32_string()
		//	* u32_to_u8_string()
		//
  		std::string s0 = u8"άλφα";
  		std::string s1 = "\\u03ac\\u03bb\\u03c6\\u03b1";	// alpha
		std::pair<utf::u32_string, bool> s2 = utf::u8_to_u32_string(s0);
		BOOST_CHECK(s2.second);
		auto s3 = utf::u32_to_u8_string(s2.first);
 		//std::cout << "s2.size(" << s2.first.size() << ")" << std::endl;
 		//for (auto c : s2.first)
 		//{
 		//	std::cout << c << std::endl;
 		//}
  	//	std::cout << s0 << " - " << s3 << std::endl;
		BOOST_CHECK_EQUAL(s0, s3);
		std::pair<utf::u32_string, bool> s4 = utf::u8_to_u32_string(s1);
		//std::cout << "s4.size(" << s4.first.size() << ")" << std::endl;
		//for (auto c : s4.first)
		//{
		//	std::cout << c << std::endl;
		//}
		BOOST_CHECK(s4.second);
		auto s5 = utf::u32_to_u8_string(s4.first);
		//std::cout << "s5.size(" << s5.size() << ")" << std::endl;
		//for (auto c : s5)
		//{
		//	std::cout << +c << std::endl;
		//}
		//std::cout << s0 << " - " << s5 << std::endl;
		//	windows implementation has serious problems here
		BOOST_CHECK_EQUAL(s0, s5);
		
		//
		//	test 
		//	* quote_parser()
		//
		std::string sq0 = u8"λάμδα";
		std::string sq1 = u8"\"" + sq0 + u8"\"";	//	the quote
		std::pair<utf::u32_string, bool> sq2 = utf::parse_utf8_quote(sq1);
		BOOST_CHECK(sq2.second);
		auto sq3 = utf::u32_to_u8_string(sq2.first);
 		std::cout << sq0 << " - " << sq3 << std::endl;
		BOOST_CHECK_EQUAL(sq0, sq3);
		
		//
		//	test 
		//	* obj_parser()
		//
		std::pair<object, bool> so0 = utf::parse_utf8_object(sq1);
		BOOST_CHECK(so0.second);
		io::serialize_plain(std::cout, so0.first);
		
		return true;
	}
	
}
