/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include "token_parser.hpp"
#include <cyng/parser/utf8_parser.h>
#include <cyng/intrinsics/buffer.h>

namespace cyng	
{

	template struct literal_parser <std::string::const_iterator>;
	template struct lit_obj_parser <std::string::const_iterator>;
// 	template struct token_parser <std::string::const_iterator>;
	
	template struct identifier_parser <std::string::const_iterator>;
	template struct identifier_parser <buffer_t::const_iterator>;
	template struct identifier_parser <utf::convert_u8_to_u32>;
	
	template struct directive_parser <std::string::const_iterator>;
	template struct directive_parser <buffer_t::const_iterator>;

	template struct literal_parser <buffer_t::const_iterator>;
	template struct lit_obj_parser <buffer_t::const_iterator>;
// 	template struct token_parser <buffer_t::const_iterator>;

// 	std::pair<tuple_t, bool > parse_token(std::string const& inp)
// 	{
// 		tuple_t result;;
// 		token_parser< std::string::const_iterator >	g;
// 		std::string::const_iterator iter = inp.begin();
// 		std::string::const_iterator end = inp.end();
// 		const bool r = boost::spirit::qi::parse(iter, end, g, result);
// 		return std::make_pair(result, r);
// 	}

}	//	namespace cyng

 
