/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include "parser.hpp"

#include <iostream>
#include <string>
#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng	
{
	namespace docscript
	{
		utf::u32_string parse_utf8(std::string const& input)
		{
			auto first(begin(input)), last(end(input));
			typedef boost::u8_to_u32_iterator<decltype(first)> Conv2Utf32;
			Conv2Utf32 f(first), l(last);
			static const utf8_parser<Conv2Utf32> g;

			//static utf8_parser< boost::u8_to_u32_iterator<const char*> >	g;
			//boost::u8_to_u32_iterator<const char*> first(input.c_str());
			//boost::u8_to_u32_iterator<const char*> last();
			//std::vector<uint32_t> result; 
			utf::u32_string result;
			//boost::spirit::qi::parse(first, last, *boost::spirit::standard_wide::char_, result);
			boost::spirit::qi::parse(f, l, g, result);
			return result;
		}

		std::string parse_quoted_string(std::string const& input)
		{
			//	prepare input
			auto first(begin(input)), last(end(input));

			typedef boost::u8_to_u32_iterator<decltype(first)> Conv2Utf32;
			Conv2Utf32 f(first), saved = f, l(last);

			static const quoted_string_parser<Conv2Utf32, boost::spirit::qi::standard_wide::space_type> p;

			std::string result;
			if (!boost::spirit::qi::phrase_parse(f, l, p, boost::spirit::qi::standard_wide::space, result))
			{
				std::cerr << "whoops at position #" << std::distance(saved, f) << "\n";
			}

			return result;
		}

		utf::u32_string parse_quote(std::string const& input)
		{
			//	prepare input
			auto first(begin(input)), last(end(input));

			typedef boost::u8_to_u32_iterator<decltype(first)> Conv2Utf32;
			Conv2Utf32 f(first), saved = f, l(last);

			//	grammar
			static const quote_parser<Conv2Utf32, boost::spirit::qi::standard_wide::space_type> p;

			utf::u32_string result;
			if (!boost::spirit::qi::phrase_parse(f, l, p, boost::spirit::qi::standard_wide::space, result))
			{
				std::cerr << "whoops at position #" << std::distance(saved, f) << "\n";
			}
			return result;
		}


		utf::u32_string_pair_t parse_pair(std::string const& input)
		{
			//	prepare input
			auto first(begin(input)), last(end(input));

			typedef boost::u8_to_u32_iterator<decltype(first)> Conv2Utf32;
			Conv2Utf32 f(first), saved = f, l(last);

			//	grammar
			static const pair_parser<Conv2Utf32, boost::spirit::qi::standard_wide::space_type> p;

			utf::u32_string_pair_t result;
			if (!boost::spirit::qi::phrase_parse(f, l, p, boost::spirit::qi::standard_wide::space, result))
			{
				std::cerr << "whoops at position #" << std::distance(saved, f) << "\n";
			}
			return result;

		}


		/**
		 * parse include directive
		 */
		template struct include_parser<std::string::const_iterator>;

		template < typename T, typename S >
		const utf::u32_string pair_parser<T, S>::true_ = parse_utf8("true");

		template < typename T, typename S >
		const utf::u32_string pair_parser<T, S>::false_ = parse_utf8("false");

	}
}	//	cyng


