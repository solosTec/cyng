/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_VERSION_HPP
#define CYNG_PARSER_VERSION_HPP

#include <cyng/parser/version_parser.h>
#include <cyng/factory/factory.hpp>

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/phoenix.hpp>
	
namespace cyng
{
	template <typename Iterator>
	ver_parser< Iterator > :: ver_parser()
		: ver_parser::base_type( r_start )
	{

		r_start
		= (r_uint16 >> '.' >> r_uint16)[boost::spirit::_val = boost::phoenix::construct<version>(boost::spirit::_1, boost::spirit::_2)]
		;
			
	}

	template <typename Iterator>
	rev_parser< Iterator > :: rev_parser()
		: rev_parser::base_type( r_start )
	{

		r_start
		= (r_uint16 >> '.' >> r_uint16 >> '.' >> r_uint16 >> '.' >> r_uint16)
		[boost::spirit::_val = boost::phoenix::construct<revision>(boost::spirit::_1, boost::spirit::_2, boost::spirit::_3, boost::spirit::_4)]
		;
			
	}

	object version_factory(std::uint16_t maj, std::uint16_t min)
	{
		return make_object<version>(maj, min);
	}

	object revision_factory(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d)
	{
		return make_object<revision>(a, b, c, d);
	}

	template <typename Iterator>
	version_parser< Iterator > :: version_parser()
		: version_parser::base_type( r_start )
	{
		r_start
		%= boost::spirit::qi::lit("v") >> r_version
		| boost::spirit::qi::lit("rev") >> r_revision
		;
			
		r_version
		= (r_uint16 >> '.' >> r_uint16)[boost::spirit::_val = boost::phoenix::bind(&version_factory, boost::spirit::_1, boost::spirit::_2)]
		;
	
 		r_revision
 		= (r_uint16 >> '.' >> r_uint16 >> '.' >> r_uint16 >> '.' >> r_uint16)
		[boost::spirit::_val = boost::phoenix::bind(&revision_factory, boost::spirit::_1, boost::spirit::_2, boost::spirit::_3, boost::spirit::_4)]
		;
			
	}
} 

#endif	


