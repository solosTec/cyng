/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include "json_parser.hpp"
#include <cyng/intrinsics/buffer.h>

namespace cyng
{
	template struct json_parser <std::string::const_iterator>;
	template struct json_parser <buffer_t::const_iterator>;


	std::pair<object, bool> parse_json(std::string const& input)
	{
// 		object result;
// 		std::string::const_iterator iter = inp.begin();
// 		std::string::const_iterator end = inp.end();
				
 		auto first(begin(input)), last(end(input));
 		utf::convert_u8_to_u32 f(first), l(last);

// 		json_parser< std::string::const_iterator >	g;
		static const json_parser<utf::convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> g;
		object result;
		const bool r = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::standard_wide::space, result);

// 		auto first(begin(input)), last(end(input));
// 		convert_u8_to_u32 f(first), l(last);
// 		static const obj_parser<convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> g;
// 
// 		object result;
// 		const bool b = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::qi::standard_wide::space, result);
// 		return std::make_pair(result, b);
		
		//	this approach generate linker errors - but shouldn't
		//	Maybe we need an approach with Conv2Utf32 and standard_wide::space
// 		typedef boost::u8_to_u32_iterator<decltype(iter)> Conv2Utf32;
// 		Conv2Utf32 f(iter), l(end);
// 		json_parser< Conv2Utf32 >	g;
// 		const bool r = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::unicode::space, result);
		
		
		return std::make_pair(result, r);

	}

	std::pair<object, bool> parse_json(buffer_t const& inp)
	{
		object result;
		json_parser< buffer_t::const_iterator >	g;
		buffer_t::const_iterator iter = inp.begin();
		buffer_t::const_iterator end = inp.end();
		//	boost::spirit::standard_wide::space
		//	boost::spirit::ascii::space
		const bool r = boost::spirit::qi::phrase_parse(iter, end, g, boost::spirit::standard_wide::space, result);
		return std::make_pair(result, r);

	}

}	//	namespace cyng

 
