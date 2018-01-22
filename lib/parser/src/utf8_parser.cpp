/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include "utf8_parser.hpp"
#include <cyng/intrinsics/buffer.h>

namespace cyng
{
	namespace utf
	{
		
		/**
		 * Convert an utf-8 string into a utf-32 one 
		 */
		std::pair<u32_string, bool> u8_to_u32_string(std::string const& input)
		{
			auto first(begin(input)), last(end(input));
			convert_u8_to_u32 f(first), l(last);
			static const string_parser<convert_u8_to_u32> g;

			u32_string result;
			const bool b = boost::spirit::qi::parse(f, l, g, result);
			return std::make_pair(result, b);
		}
		
		std::string u32_to_u8_string(u32_string const& input)
		{
			return std::string(convert_u32_to_u8(input.begin()), convert_u32_to_u8(input.end()));			
		}
		
		std::pair<utf::u32_string, bool> parse_utf8_quote(std::string const& input)
		{
			auto first(begin(input)), last(end(input));
			convert_u8_to_u32 f(first), l(last);
			static const quote_parser<convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> g;

			utf::u32_string result;
			const bool b = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::qi::standard_wide::space, result);
			return std::make_pair(result, b);
		}
		
		u32_string_pair_t parse_pair(std::string const& input)
		{
			//	prepare input
			auto first(begin(input)), last(end(input));
			convert_u8_to_u32 f(first), l(last);
			auto saved = f;
			
// 			typedef boost::u8_to_u32_iterator<decltype(first)> Conv2Utf32;
// 			Conv2Utf32 f(first), saved = f, l(last);

			//	grammar
			static const pair_parser<convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> p;

			u32_string_pair_t result;
			const bool b = boost::spirit::qi::phrase_parse(f, l, p, boost::spirit::qi::standard_wide::space, result);
			if (!b)
 			{
 				std::cerr << "whoops at position #" << std::distance(saved, f) << "\n";
 			}
			return result;

		}
		
		std::pair<object, bool> parse_utf8_object(std::string const& input)
		{
			auto first(begin(input)), last(end(input));
			convert_u8_to_u32 f(first), l(last);
			static const obj_quote_parser<convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> g;

			object result;
			const bool b = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::qi::standard_wide::space, result);
			return std::make_pair(result, b);
		}
		
 		template struct char_parser <std::string::const_iterator>;
 		template struct string_parser <std::string::const_iterator>;
//  		template struct pair_parser <std::string::const_iterator>;
//  		template struct pair_parser <buffer_t::const_iterator>;
		template struct quote_parser <std::string::const_iterator>;
		template struct obj_quote_parser <std::string::const_iterator>;
		template struct quote_parser <buffer_t::const_iterator>;
		template struct obj_quote_parser <buffer_t::const_iterator>;
		// 		template struct quote_parser <std::string::const_iterator, boost::spirit::qi::standard_wide::space_type>;
		//  		template struct quote_parser <std::string::const_iterator, boost::spirit::char_encoding::standard_wide>;
		//		template struct obj_parser <std::string::const_iterator, boost::spirit::char_encoding::standard_wide>;
		
		template < typename T, typename S >
		const u32_string pair_parser<T, S>::true_ = u8_to_u32_string("true").first;

		template < typename T, typename S >
		const u32_string pair_parser<T, S>::false_ = u8_to_u32_string("false").first;
		

	}
}	//	namespace cyng

 
