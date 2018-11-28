/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_MAC_H
#define CYNG_PARSER_MAC_H

#include <cyng/object.h>
#include <cyng/intrinsics/mac.h>

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>

namespace cyng	
{
	
	/**
	 * Converts a string of hex values into a buffer.
	 */
	std::pair<mac48, bool > parse_mac48(std::string const& inp);
	std::pair<mac64, bool > parse_mac64(std::string const& inp);
	object parse_mac_obj(std::string const& inp);


	/**
	 * Accept a string of the form 00:0c:29:cc:e3:d4
	 */
	template <typename Iterator>
	struct mac48_parser
	: boost::spirit::qi::grammar<Iterator, mac48()>
	{
		mac48_parser();
		boost::spirit::qi::rule<Iterator, mac48()> r_start;
		/*
	 	 * Requires exactly two digits
		 *	T, Radix, MinDigits, MaxDigits
		 */
		boost::spirit::qi::uint_parser<unsigned int, 16, 2, 2>		r_hex2;
	};

		
	/**
	 * Accept a string of the form 70B3:D538:7000:002D or 70B3D5387000002D
	 */
	template <typename Iterator>
	struct mac64_parser
		: boost::spirit::qi::grammar<Iterator, mac64()>
	{
		mac64_parser();
		boost::spirit::qi::rule<Iterator, mac64()> r_start;

		/*
		 * Requires exactly two digits
		 *	T, Radix, MinDigits, MaxDigits
		 */
		boost::spirit::qi::uint_parser<std::uint16_t, 16, 4, 4>		r_hex4;
	};

	/**
	 * Accept a string with mac48 and mac64 format.
	 * Returns a object of type mac48 or mac64.
	 */
	template <typename Iterator>
	struct mac_obj_parser
	: boost::spirit::qi::grammar<Iterator, object()>
	{
		mac_obj_parser();
		boost::spirit::qi::rule<Iterator, object()> r_start;
		mac48_parser<Iterator>	r_mac48;
		mac64_parser<Iterator>	r_mac64;

		/**
		 * Requires exactly two digits
		 *	T, Radix, MinDigits, MaxDigits
		 */
		boost::spirit::qi::uint_parser<unsigned int, 16, 2, 2>		r_hex2;

		object mac48_helper(mac48 const& v);
		object mac64_helper(mac64 const& v);

	};
} 

#endif	

