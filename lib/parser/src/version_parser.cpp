/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include "version_parser.hpp"
#include <cyng/intrinsics/buffer.h>
#include <cyng/parser/utf8_parser.h>

namespace cyng
{
	template struct version_parser <std::string::const_iterator>;
	template struct version_parser <buffer_t::const_iterator>;
	template struct version_parser <utf::convert_u8_to_u32>;

	template struct ver_parser <std::string::const_iterator>;
	template struct rev_parser <std::string::const_iterator>;
	template struct rev_parser <utf::convert_u8_to_u32>;

	std::pair<version, bool> parse_version(std::string inp)
	{
		version result(0, 0);
		ver_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}
		
	std::pair<revision, bool> parse_revision(std::string inp)
	{
		revision result(0);
		rev_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}
		
}

 
