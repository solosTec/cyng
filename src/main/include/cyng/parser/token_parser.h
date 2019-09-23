/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_TOKEN_H
#define CYNG_PARSER_TOKEN_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <cyng/parser/utf8_parser.h>
#include <cyng/intrinsics/sets.h>

namespace cyng	
{
	
	/**
	 * parse a string with cyng syntax
	 */
	std::pair<tuple_t, bool > parse_token(std::string const&);

	/**
	 *	alnum_parser: ....
	 *	Parse any literal consisting of alphanumerical characters
	 *	and the symbols '.' (dot) and '_' (underline) in any order.
	 */
	template <typename Iterator>
	struct literal_parser
		: boost::spirit::qi::grammar<Iterator, std::string()>
	{
		literal_parser();
		boost::spirit::qi::rule<Iterator, std::string()> r_start;
	};
	
	/**
	 * Accepts all strings starting with a alphabetic character or and underline
	 * followed by alphanumeric characters, underlines or dots.
	 * Is equivalent to
	 * @code
	 boost::spirit::qi::char_("a-zA-Z_") >> *boost::spirit::qi::char_("a-zA-Z_.0-9")
	 * @endcode
	 */
	template <typename Iterator>
	struct identifier_parser
		: boost::spirit::qi::grammar<Iterator, std::string()>
	{
		identifier_parser();
		boost::spirit::qi::rule<Iterator, std::string()> r_start;
		boost::spirit::qi::rule<Iterator, char()>	r_first, r_other;
	};

	/**
	 * The same as an identifier_parser but accepts '-' in the middle
	 * of the string.
	 */
	template <typename Iterator>
	struct directive_parser
		: boost::spirit::qi::grammar<Iterator, std::string()>
	{
		directive_parser();
		boost::spirit::qi::rule<Iterator, std::string()> r_start;
		boost::spirit::qi::rule<Iterator, char()>	r_first, r_other;
	};

	/**
	 * Accept all alphanumeric values and "_" and "."
	 */
	template <typename Iterator>
	struct lit_obj_parser
		: boost::spirit::qi::grammar<Iterator, cyng::object()>
	{
		lit_obj_parser();
		boost::spirit::qi::rule<Iterator, cyng::object()> r_start;
		literal_parser<Iterator>	r_literal;
	};
	
} 	//	cyng

#endif	// CYNG_PARSER_TOKEN_H

