/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher
 *
 */ 

#ifndef CYNG_CSV_PARSER_H
#define CYNG_CSV_PARSER_H

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
	std::pair<tuple_t, bool> parse_csv(std::string const& inp);

	/**
	 * Parse a buffer as cyng::object.
	 */
	std::pair<tuple_t, bool> parse_csv(buffer_t const& inp);

	/**
	 * Parse CSV data into a list of CYY objects. 
	 * This demonstrates quite impressively, how powerful 
	 * the CYY object system could be.
	 * 
	 * Contains support for date time with following format:
	 @code
	 * "Capture Time": @2016-08-01T18:15:00Z
	 @endcode
	 *
	 * Values are separated by ',' or ';'
	 *
	 * grammar:
	 *   line   = column % colsep;
     *   column = number | quoted | datetime | UUID | literal;
	 */
	template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
	struct csv_parser
	: boost::spirit::qi::grammar<Iterator, tuple_t(), Skipper>
	{
		csv_parser();
		boost::spirit::qi::rule<Iterator, tuple_t(), Skipper>	r_csv, p_, r_line;
		boost::spirit::qi::rule<Iterator, object(), Skipper>	r_value;

		numeric_parser<Iterator>	r_numeric;
		utf::obj_quote_parser<Iterator>	r_string;	//	object
		utf::quote_parser<Iterator>	r_quote;		//	u32_string
		identifier_parser<Iterator>	r_ident;	//	std::string
		rfc3339_obj_parser<Iterator>	r_dt;	//	std::chrono::system_clock::time_point

		//	auto detect UUIDs
		boost::spirit::qi::rule<Iterator, object(), Skipper> 	r_uuid_obj;
 		boost::spirit::qi::rule<Iterator, buffer_t()> 	r_uuid;
		boost::spirit::qi::uint_parser<unsigned int, 16, 2, 2>	r_hex2;

	};
} 	//	cyng

#endif	

