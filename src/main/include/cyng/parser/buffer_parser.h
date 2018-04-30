/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Sylko Olzscher
 *
 */

#ifndef CYNG_PARSER_BUFFER_H
#define CYNG_PARSER_BUFFER_H


#include <cyng/object.h>
#include <cyng/intrinsics/buffer.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

namespace cyng
{
	
	/**
	 * Converts a string of hex values into a buffer.
	 * Input length must be even.
	 *
	 * Example:
	 * @code
	 * const std::pair<buffer_t, bool > r = parse_hex_string("aabbccddeeff");
	 * @endcode
	 */
	std::pair<buffer_t, bool > parse_hex_string(std::string const& inp);

	/**
	* Parse a string of hex values that are separated by spaces.
	* Example:
	* @code
	* const std::pair<buffer_t, bool > r = parse_hex_string("aa bb cc dd ee ff");
	* @endcode
	*/
	std::pair<buffer_t, bool > parse_log_string(std::string const& inp);

	/**
	 *	Parse a string of hex values that are not separated by spaces.
	 *	Input length hast to be even.
	 */
	template <typename Iterator>
	struct hex_string : boost::spirit::qi::grammar<Iterator, buffer_t()>
	{
		hex_string();
		boost::spirit::qi::rule<Iterator, buffer_t()> r_start;

		/**
		 * Requires exactly two digits
		 * T, Radix, MinDigits, MaxDigits
		 */
		boost::spirit::qi::uint_parser<char, 16, 2, 2>		r_hex2;
	};

	template <typename Iterator>
	struct buffer
	: boost::spirit::qi::grammar<Iterator, buffer_t()>
	{
		buffer();
		boost::spirit::qi::rule<Iterator, buffer_t()> r_start;
		/**
		 * Requires exactly two digits
		 *	T, Radix, MinDigits, MaxDigits
		 */
		boost::spirit::qi::uint_parser<char, 16, 2, 2>		r_hex2;
	};

	template <typename Iterator>
	struct buffer_parser
	: boost::spirit::qi::grammar<Iterator, object()>
	{
		buffer_parser();
		boost::spirit::qi::rule<Iterator, object()> 	r_start;
		buffer<Iterator>	r_buffer;
			
		/**
	 	 * This is a workaround since boost::phoenix::bind() cannot
		 * deal with Rvalues.
		 */
		object helper(buffer_t const&); 
	};

	/**
	 *	Simple parser for a list of hex values separated by spaces
	 *	Example: 01 FF ab 27
	 *
	 *	To parse a list without spaces use parse_hex_string().
	 */
	template <typename Iterator>
	struct log_parser
		: boost::spirit::qi::grammar<Iterator, buffer_t()>
	{
		log_parser();
		boost::spirit::qi::rule<Iterator, buffer_t()> r_buffer;
		boost::spirit::qi::uint_parser<char, 16, 2, 2>		r_hex2;
	};

}

#endif

