/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include "csv_parser.hpp"
#include <cyng/intrinsics/buffer.h>

namespace cyng
{
	template struct csv_parser <std::string::const_iterator>;
	template struct csv_parser <buffer_t::const_iterator>;


	std::pair<tuple_t, bool> parse_csv(std::string const& input)
	{
 		auto first(begin(input)), last(end(input));
 		utf::convert_u8_to_u32 f(first), l(last);

		static const csv_parser<utf::convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> g;
		tuple_t result;
		const bool r = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::standard_wide::space, result);
		
		return std::make_pair(result, r);
	}

	std::pair<tuple_t, bool> parse_csv(buffer_t const& inp)
	{
		csv_parser< buffer_t::const_iterator >	g;
		buffer_t::const_iterator iter = inp.begin();
		buffer_t::const_iterator end = inp.end();
		tuple_t result;
		const bool r = boost::spirit::qi::phrase_parse(iter, end, g, boost::spirit::standard_wide::space, result);
		return std::make_pair(result, r);

	}

}	//	namespace cyng

 
