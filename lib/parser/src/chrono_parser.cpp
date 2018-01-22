/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include "chrono_parser.hpp"
#include <cyng/parser/utf8_parser.h>
#include <cyng/intrinsics/buffer.h>
#include <cyng/factory.h>

namespace cyng	
{
	template struct chrono_parser <std::string::const_iterator>;
	template struct chrono_parser <buffer_t::const_iterator>;
	template struct chrono_parser <utf::convert_u8_to_u32>;
	template struct db_timestamp_parser <std::string::const_iterator>;
	template struct timespan_parser <std::string::const_iterator>;
	template struct timespan_parser <utf::convert_u8_to_u32>;
	template struct rfc3339_timestamp_parser <std::string::const_iterator>;
	template struct rfc3339_timestamp_parser <buffer_t::const_iterator>;
	template struct rfc3339_obj_parser <std::string::const_iterator>;
	template struct rfc3339_obj_parser <buffer_t::const_iterator>;
	template struct rfc3339_obj_parser <utf::convert_u8_to_u32>;

	std::pair<object, bool > parse_basic_timestamp(std::string const& inp)
	{
		object result = make_now();
		
		auto first(begin(inp)), last(end(inp));
		utf::convert_u8_to_u32 f(first), l(last);
		static const timepoint_basic_parser<utf::convert_u8_to_u32, boost::spirit::qi::standard_wide::space_type> g;
		const bool r = boost::spirit::qi::phrase_parse(f, l, g, boost::spirit::qi::standard_wide::space, result);
		
		return (r)
			? std::make_pair(result, r)
			: std::make_pair(make_object(), r)
			;
	}

	std::pair<std::chrono::system_clock::time_point, bool > parse_db_timestamp(std::string const& inp)
	{
		std::chrono::system_clock::time_point result = std::chrono::system_clock::now();
		db_timestamp_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}
	
	std::pair<std::chrono::system_clock::time_point, bool > parse_rfc3339_timestamp(std::string const& inp)
	{
		std::chrono::system_clock::time_point result = std::chrono::system_clock::now();
		rfc3339_timestamp_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}

	std::pair<std::chrono::microseconds, bool > parse_timespan(std::string const& inp)
	{
		std::chrono::microseconds result(0);
		timespan_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}
	
	object parse_rfc3339_obj(std::string const& inp)
	{
		object obj;
		rfc3339_obj_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, obj);
		return (r)
			? obj
			: make_object()
			;

	}

}	//	namespace cyng

 
