/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_OBJECT_H
#define CYNG_PARSER_OBJECT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cyng/object.h>

#include <cyng/parser/utf8_parser.h>
#include <cyng/parser/chrono_parser.h>
#include <cyng/parser/numeric_parser.h>
#include <cyng/parser/chrono_parser.h>
#include <cyng/intrinsics/sets.h>

namespace cyng	
{

	/**
	 * uint_parser<> has the following template parameters:
	 * numeric type, radix base, minimum number of digits, maximum number
	 * of digits.
	 */
	template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
	struct object_parser
	: boost::spirit::qi::grammar<Iterator, object(), Skipper>
	{
		object_parser();
		boost::spirit::qi::rule<Iterator, object(), Skipper> 	r_start, r_obj, r_set, r_map;
		chrono_parser<Iterator, Skipper>		r_chrono;
		numeric_parser<Iterator>	r_numeric;
		utf::string_parser<Iterator>	r_string;
		utf::obj_quote_parser<Iterator>	r_quote;
		
		/**
		 * size indicator
		 */
		boost::spirit::qi::uint_parser<std::size_t, 10, 1, -1> 	r_size;
		
		
		boost::spirit::qi::rule<Iterator, tuple_t(), Skipper>	r_tuple;
		
 		boost::spirit::qi::rule<Iterator, boost::fusion::vector<std::size_t, object>(), Skipper> r_attr;
		boost::spirit::qi::rule<Iterator, boost::fusion::vector<utf::u32_string, object>(), Skipper> r_param;

		boost::spirit::qi::rule<Iterator, vector_t(), Skipper>	r_pmap, r_amap;
		

	};

} 	//	cyng

#endif	// CYNG_PARSER_OBJECT_H


