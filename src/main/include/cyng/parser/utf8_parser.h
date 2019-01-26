/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_UTF8_H
#define CYNG_PARSER_UTF8_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/regex/pending/unicode_iterator.hpp>
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <cyng/object.h>

namespace cyng	
{
	namespace utf 
	{
		/**
		 * a UTF-32 string to hold results from unicode (UTF-8) strings
		 */
		using u32_string = std::u32string;

		/**
		 * define a pair of UTF-32 strings
		 */
		using u32_string_pair_t = std::pair<u32_string, u32_string>;
		
		/**
		 * Define a converter back from utf-32 to utf-8 strings 
		 */
		using convert_u32_to_u8 = boost::u32_to_u8_iterator<u32_string::const_iterator>;
		
		/**
		 * Define a converter from utf-8 to utf-32 strings 
		 */
		using convert_u8_to_u32 = boost::u8_to_u32_iterator<std::string::const_iterator>;

		/**
		 * Converts a UTF-8 string into a UTF-32 string using the utf_parser
		 */
		std::pair<u32_string, bool> u8_to_u32_string(std::string const& input);
		
		/**
		 * read a UTF-8 encoded quote
		 */
		std::pair<utf::u32_string, bool> parse_utf8_quote(std::string const& input);
		
		/**
		 * read a UTF-8 encoded quote as object of type string
		 */
		std::pair<object, bool> parse_utf8_object(std::string const& input);

		/**
		 * Helper function to convert an utf-32 string into an utf-8 string.
		 * This is a simple conversion - not a real parser.
		 */
		std::string u32_to_u8_string(u32_string const& input);

		/**
		 * Parse a pair of strings separated by an colon (:)
		 */
		u32_string_pair_t parse_pair(std::string const& input);
		
		/**
		 * parse a single UTF-8 char into a UTF-32 representation (std::uint32_t)
		 */
		template <typename Iterator>
		struct char_parser : boost::spirit::qi::grammar<Iterator, u32_string()>
		{
			char_parser();
			boost::spirit::qi::rule<Iterator, u32_string()> r_start, r_char;
			boost::spirit::qi::uint_parser<std::uint32_t, 16, 4, 4> r_4hex_digit;
		};
		
		/**
		 * Converts a UTF-8 string into a UTF-32 string
		 */
		template <typename Iterator>
		struct string_parser : boost::spirit::qi::grammar< Iterator, u32_string()>
		{
			string_parser();
			boost::spirit::qi::rule<Iterator, u32_string()>	r_start;
			char_parser<Iterator>	r_char;
		};

		/**
		 *  Simple quote parser
		 */
		template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
		struct quote_parser : boost::spirit::qi::grammar< Iterator, u32_string(), Skipper >
		{
			quote_parser();
			boost::spirit::qi::rule<Iterator, u32_string(), Skipper>	r_start;
			string_parser<Iterator>	r_string;
		};
		
		/**
		 * Parse a pair of strings separated by an colon (:)
		 */
		template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
		struct pair_parser : boost::spirit::qi::grammar<Iterator, u32_string_pair_t(), Skipper>
		{
			pair_parser();
			boost::spirit::qi::rule<Iterator, u32_string_pair_t(), Skipper> r_start;
			boost::spirit::qi::rule<Iterator, boost::fusion::vector<u32_string, u32_string>(), Skipper > r_pair;
			boost::spirit::qi::rule<Iterator, u32_string(), Skipper> r_value;
			quote_parser<Iterator>	r_quote;
			static const u32_string true_, false_;
		};


		/**
		 *  Read a utf-8 quote and produce an object of type string
		 */
		template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
		struct obj_quote_parser : boost::spirit::qi::grammar< Iterator, object(), Skipper >
		{
			obj_quote_parser();
			boost::spirit::qi::rule<Iterator, object(), Skipper> 	r_start;
			quote_parser<Iterator, Skipper>	r_quote;
		};

	}	//	utf
} 	//	cyng

#endif	// CYNG_PARSER_UTF8_H


