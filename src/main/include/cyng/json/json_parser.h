/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_PARSER_H
#define CYNG_JSON_PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/intrinsics/sets.h>
#include <cyng/intrinsics/buffer.h>
// #define BOOST_SPIRIT_UNICODE
#include <cyng/parser/numeric_parser.h>
#include <cyng/parser/utf8_parser.h>
#include <cyng/parser/token_parser.h>
#include <cyng/parser/chrono_parser.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/container/vector.hpp>

namespace cyng	
{

	/**
	 * Parse a string as cyng::object.
	 */
	std::pair<object, bool> parse_json(std::string const& inp);

	/**
	 * Parse a buffer as cyng::object.
	 */
	std::pair<object, bool> parse_json(buffer_t const& inp);

	/**
		* Parse JSON data into a tree of CYY objects. 
		* This demonstrates quite impressively, how powerful 
		* the CYY object system could be.
		* 
		* Contains support for date time with following format:
		@code
		* "Capture Time": @2016-08-01T18:15:00Z
		@endcode
		*
		* Furthermore the parser accepts objects names without quotation marks.
		*/
	template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
	struct json_parser
	: boost::spirit::qi::grammar<Iterator, object(), Skipper>
	{
		json_parser();
		boost::spirit::qi::rule<Iterator, object(), Skipper>	r_json, p_, r_obj, r_value;
		boost::spirit::qi::rule<Iterator, vector_t(), Skipper>	r_vector;
		boost::spirit::qi::rule<Iterator, tuple_t(), Skipper>	r_members;
		boost::spirit::qi::rule<Iterator, boost::fusion::vector<utf::u32_string, object>(), Skipper > r_pair;

		numeric_parser<Iterator>	r_numeric;
		utf::obj_quote_parser<Iterator>	r_string;	//	object
		utf::quote_parser<Iterator>	r_quote;		//	u32_string
		identifier_parser<Iterator>	r_ident;	//	std::string
		rfc3339_obj_parser<Iterator>	r_dt;	//	std::chrono::system_clock::time_point

		//	auto detect UUIDs
		boost::spirit::qi::rule<Iterator, object(), Skipper> 	r_uuid_obj;
 		boost::spirit::qi::rule<Iterator, buffer_t()> 	r_uuid;
		boost::spirit::qi::uint_parser<char, 16, 2, 2>	r_hex2;

	};
} 	//	cyng

#endif	// CYNG_JSON_PARSER_H

