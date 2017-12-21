/*
 * Copyright Sylko Olzscher 2016
 *
 * Use, modification, and distribution is subject to the Boost Software
 * License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef CYX_DOCSCRIPT_PARSER_H
#define CYX_DOCSCRIPT_PARSER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200) 

// #define BOOST_SPIRIT_UNICODE
#include <boost/regex/pending/unicode_iterator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/range.hpp>
#include <boost/filesystem.hpp>
#include <iterator>
#include <iostream>
#include <ostream>
#include <vector>
#include <cyng/docscript/docscript.h>


namespace cyng
{
	namespace docscript
	{
		//	@see http://stackoverflow.com/questions/19273882/handling-utf-8-in-boost-spirit-with-utf-32-parser
		//
		template <typename It, typename Skipper = boost::spirit::qi::standard_wide::space_type>
		struct quoted_string_parser : boost::spirit::qi::grammar<It, std::string(), Skipper>
		{
			quoted_string_parser();
			boost::spirit::qi::rule<It, std::string(), Skipper> r_start;
			boost::spirit::qi::rule<It, utf::u32_string()> r_string, r_char;
			boost::spirit::qi::uint_parser<uint32_t, 16, 4, 4> _4HEXDIG;
		};

		std::string parse_quoted_string(std::string const& input);

		//bool parse_utf8(std::vector<std::uint32_t>& result
		//	, boost::u8_to_u32_iterator<const char*>
		//	, boost::u8_to_u32_iterator<const char*>);

		//template <typename Iterator>
		//struct utf8_parser : boost::spirit::qi::grammar< Iterator, std::vector<std::uint32_t>() >
		//{
		//	utf8_parser();

		//	boost::spirit::qi::rule<Iterator, std::vector<std::uint32_t>() >	r_start;
		//};

		/**
		 * Converts a UTF-8 string into a UTF-32 string
		 */
		template <typename Iterator>
		struct utf8_parser : boost::spirit::qi::grammar< Iterator, utf::u32_string() >
		{
			utf8_parser();
			boost::spirit::qi::rule<Iterator, utf::u32_string() >	r_start;
		};

		/**
		 * Converts a UTF-8 string into a UTF-32 string
		 */
		utf::u32_string parse_utf8(std::string const& input);

		/**
		 * Simple quote parser without any escape symbols. It accepts any UTF-8 string that starts
		 * and ends with a quotation mark. The quotation mark itself is stripped from the result.
		 * It's not possible to place a quotation mark inside the quote.
		 */
		template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
		struct quote_parser : boost::spirit::qi::grammar<Iterator, utf::u32_string(), Skipper>
		{
			quote_parser();
			boost::spirit::qi::rule<Iterator, utf::u32_string(), Skipper> r_start;
			boost::spirit::qi::rule<Iterator, utf::u32_string()> r_string;
			boost::spirit::qi::rule<Iterator, std::uint32_t()> r_char;
		};

		 utf::u32_string parse_quote(std::string const& input);

		template <typename Iterator, typename Skipper = boost::spirit::qi::standard_wide::space_type>
		struct pair_parser : boost::spirit::qi::grammar<Iterator, utf::u32_string_pair_t(), Skipper>
		{
			pair_parser();
			boost::spirit::qi::rule<Iterator, utf::u32_string_pair_t(), Skipper> r_start;
			boost::spirit::qi::rule<Iterator, boost::fusion::vector<utf::u32_string, utf::u32_string>(), Skipper > r_pair;
			boost::spirit::qi::rule<Iterator, utf::u32_string(), Skipper> r_value;
			quote_parser<Iterator>	r_quote;
			static const utf::u32_string true_, false_;
		};

		utf::u32_string_pair_t parse_pair(std::string const& input);


		template <typename Iterator>
		struct include_parser : boost::spirit::qi::grammar< Iterator, boost::filesystem::path() >
		{
			include_parser();

			boost::spirit::qi::rule<Iterator, boost::filesystem::path() >	r_start;
			boost::spirit::qi::rule<Iterator, std::string()> r_path;


		};
	}
}	//	cyng

#endif	//	CYX_DOCSCRIPT_PARSER_H
