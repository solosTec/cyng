/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_MAC_HPP
#define CYNG_PARSER_MAC_HPP

#include <cyng/parser/mac_parser.h>
#include <cyng/factory/factory.hpp>

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyy::object
	
namespace cyng
{
	template <typename Iterator>
	mac48_parser< Iterator > ::mac48_parser()
		: mac48_parser::base_type( r_start )
	{
		//	00:0c:29:cc:e3:d4
		r_start	
		= (r_hex2
			>> ':' 
			>> r_hex2
			>> ':'
			>> r_hex2
			>> ':'
			>> r_hex2
			>> ':'
			>> r_hex2
			>> ':'
			>> r_hex2)
		[boost::spirit::_val = boost::phoenix::construct<mac48>(boost::spirit::_1, boost::spirit::_2, boost::spirit::_3, boost::spirit::_4, boost::spirit::_5, boost::spirit::_6)]
		| (r_hex2
			>> '-'
			>> r_hex2
			>> '-'
			>> r_hex2
			>> '-'
			>> r_hex2
			>> '-'
			>> r_hex2
			>> '-'
			>> r_hex2)
			[boost::spirit::_val = boost::phoenix::construct<mac48>(boost::spirit::_1, boost::spirit::_2, boost::spirit::_3, boost::spirit::_4, boost::spirit::_5, boost::spirit::_6)]
		;
	};
		
	template <typename Iterator>
	mac64_parser< Iterator > ::mac64_parser()
		: mac64_parser::base_type(r_start)
	{
		//	70B3:D538:7000:002D
		r_start
			= (r_hex4
			>> *boost::spirit::qi::lit(':')
			>> r_hex4
			>> *boost::spirit::qi::lit(':')
			>> r_hex4
			>> *boost::spirit::qi::lit(':')
			>> r_hex4)
			[boost::spirit::_val = boost::phoenix::construct<mac64>(boost::spirit::_1, boost::spirit::_2, boost::spirit::_3, boost::spirit::_4)]
		;
	};

	template <typename Iterator>
	mac_obj_parser< Iterator > :: mac_obj_parser()
		: mac_obj_parser::base_type( r_start )
	{
		//	00:0c:29:cc:e3:d4
		r_start
			= r_mac64[boost::spirit::_val = boost::phoenix::bind(&mac_obj_parser<Iterator>::mac64_helper, this, boost::spirit::_1)]
			| r_mac48[boost::spirit::_val = boost::phoenix::bind(&mac_obj_parser<Iterator>::mac48_helper, this, boost::spirit::_1)]
			;

	};

	template <typename Iterator>
	object mac_obj_parser< Iterator > :: mac48_helper(mac48 const& v)
	{
		return make_object(v);
	}

	template <typename Iterator>
	object mac_obj_parser< Iterator > :: mac64_helper(mac64 const& v)
	{
		return make_object(v);
	}

}

#endif


