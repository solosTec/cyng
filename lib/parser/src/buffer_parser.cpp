/*
* The MIT License (MIT)
*
* Copyright (c) 2017 Sylko Olzscher
*
*/

#include "buffer_parser.hpp"
#include <cyng/intrinsics/buffer.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/parser/utf8_parser.h>

namespace cyng	
{

	template struct buffer_parser <std::string::const_iterator>;
	template struct buffer_parser <buffer_t::const_iterator>;
	template struct buffer_parser <utf::convert_u8_to_u32>;

	template struct log_parser <std::string::const_iterator>;
	template struct log_parser <buffer_t::const_iterator>;
	template struct log_parser <utf::convert_u8_to_u32>;

	std::pair<buffer_t, bool > parse_hex_string(std::string const& inp)
	{
		BOOST_ASSERT_MSG(inp.size() % 2 == 0, "input length must be even");
		buffer_t result;
		hex_string< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}

	std::pair<buffer_t, bool > parse_log_string(std::string const& inp)
	{
		buffer_t result;
		log_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}
		
}

 
