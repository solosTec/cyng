/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Sylko Olzscher
 *
 */

#ifndef CYNG_PARSER_BUFFER_HPP
#define CYNG_PARSER_BUFFER_HPP

#include <cyng/parser/buffer_parser.h>
#include <cyng/factory/factory.hpp>

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyy::object
	
namespace cyng
{

	template <typename Iterator>
	hex_string< Iterator >::hex_string()
		: hex_string::base_type(r_start)
	{
		r_start
			%= (*r_hex2)
			;
	};

	template <typename Iterator>
	buffer< Iterator > :: buffer()
		: buffer::base_type( r_start )
	{
		r_start	
		%= *(boost::spirit::qi::alnum | boost::spirit::qi::space | "\\x" >> r_hex2)
		;
	};
		
	template <typename Iterator>
	object buffer_parser< Iterator > :: helper(buffer_t const& buffer)
	{
		return make_object(buffer);
	}
		
	template <typename Iterator>
	buffer_parser< Iterator > :: buffer_parser()
		: buffer_parser::base_type( r_start )
	{
		/**
		 * buffers are surrounded by single quotation marks and contain any symbol
		 * or an escape sequence
		 */
		r_start
			= ('\'' >> r_buffer  >> '\'')[boost::spirit::_val = boost::phoenix::bind(&buffer_parser<Iterator>::helper, this, boost::spirit::_1)]
			;
	}

	template <typename Iterator>
	log_parser< Iterator > ::log_parser()
		: log_parser::base_type(r_buffer)
	{
		//
		//	boost::spirit::qi::hex => unsigned int
		//
		r_buffer
			%= r_hex2 % (+boost::spirit::qi::space)
			//				%= boost::spirit::qi::hex % (+boost::spirit::qi::space)
			;
	}

}

#endif


