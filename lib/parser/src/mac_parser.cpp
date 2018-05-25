/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include "mac_parser.hpp"
#include <cyng/factory/factory.hpp>

namespace cyng
{

	template struct mac48_parser <std::string::const_iterator>;
	template struct mac64_parser <std::string::const_iterator>;
	template struct mac_obj_parser <std::string::const_iterator>;

	std::pair<mac48, bool > parse_mac48(std::string const& inp)
	{
		mac48 result;
		mac48_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}
		
	std::pair<mac64, bool > parse_mac64(std::string const& inp)
	{
		mac64 result;
		mac64_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, result);
		return std::make_pair(result, r);
	}

	object parse_mac_obj(std::string const& inp)
	{
		object obj;
		mac_obj_parser< std::string::const_iterator >	g;
		std::string::const_iterator iter = inp.begin();
		std::string::const_iterator end = inp.end();
		const bool r = boost::spirit::qi::parse(iter, end, g, obj);
		return (r)
			? obj
			: make_object()
			;

	}
}

 
