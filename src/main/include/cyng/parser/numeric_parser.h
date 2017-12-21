/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_NUMERIC_H
#define CYNG_PARSER_NUMERIC_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/object.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

namespace cyng	
{

	/**
	 * uint_parser<> has the following template parameters:
	 * numeric type, radix base, minimum number of digits, maximum number
	 * of digits.
	 */
	template <typename Iterator>
	struct numeric_parser
	: boost::spirit::qi::grammar<Iterator, object()>
	{
		numeric_parser();
		boost::spirit::qi::rule<Iterator, object()> 	r_start;
		boost::spirit::qi::rule<Iterator, object()>  	r_int;
		
		boost::spirit::qi::uint_parser<std::uint8_t, 10, 1, 3>		r_uint8;
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 1, 5> 	r_uint16;
		boost::spirit::qi::uint_parser<std::uint32_t, 10, 1, 10> 	r_uint32;
		boost::spirit::qi::uint_parser<std::uint64_t, 10, 1, -1> 	r_uint64;

		boost::spirit::qi::int_parser<std::int8_t, 10, 1, 3>		r_int8;
		boost::spirit::qi::int_parser<std::int16_t, 10, 1, -1> 		r_int16;
		boost::spirit::qi::int_parser<std::int32_t, 10, 1, -1> 		r_int32;
		boost::spirit::qi::int_parser<std::int64_t, 10, 1, -1> 		r_int64;
		
		boost::spirit::qi::real_parser<double, boost::spirit::qi::strict_real_policies<double> > r_strict_double;

	};

} 	//	cyng

#endif	// CYNG_PARSER_NUMERIC_H

