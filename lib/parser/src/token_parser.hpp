/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_TOKEN_HPP
#define CYNG_PARSER_TOKEN_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/parser/token_parser.h>
#include <cyng/factory.h>

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyng::object
	
namespace boost		
{ 
	namespace spirit	
	{ 
		namespace traits	
		{ 
	
			template <> 
			struct transform_attribute< cyng::object, std::string, boost::spirit::qi::domain > 
			{ 
				typedef std::string type; 
				static type pre(cyng::object& v) 
				{ 
					return type(); 
				} 
				static void post(cyng::object& val, type const& attr) 
				{
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) 
				{}
			}; 
			
		}	//	traits
	}	//	spirit
}	//	boost

namespace cyng	
{
	template <typename Iterator>
	literal_parser< Iterator >::literal_parser()
		: literal_parser::base_type(r_start)
	{
		r_start
			%= *(boost::spirit::qi::alnum | boost::spirit::qi::ascii::char_("_."))
			;
	}
	
	template <typename Iterator>
	lit_obj_parser< Iterator >::lit_obj_parser()
		: lit_obj_parser::base_type(r_start)
	{
		r_start
			= r_literal	//	implicit conversion
			;
	}
	
	template <typename Iterator>
	identifier_parser< Iterator >::identifier_parser()
		: identifier_parser::base_type(r_start)
	{
		//	lexeme directive ist required not to match
		//	the tokens after the whitespace
		//
		r_start
			%= boost::spirit::qi::lexeme[+r_first >> *r_other]
			;
		
		r_first
			%= boost::spirit::qi::alpha
			| boost::spirit::qi::ascii::char_("_")
			;

		r_other
			%= boost::spirit::qi::alnum
			| boost::spirit::qi::ascii::char_("_.")
			;
	}

	template <typename Iterator>
	directive_parser< Iterator >::directive_parser()
		: directive_parser::base_type(r_start)
	{
		//	lexeme directive ist required not to match
		//	the tokens after the whitespace
		//
		r_start
			%= boost::spirit::qi::lexeme[+r_first >> *r_other]
			;

		r_first
			%= boost::spirit::qi::alpha
			| boost::spirit::qi::ascii::char_("_")
			;

		r_other
			%= boost::spirit::qi::alnum
			| boost::spirit::qi::ascii::char_("_.")
			| boost::spirit::qi::ascii::char_('-')
			;
	}

// 	template <typename Iterator>
// 	token_parser< Iterator >::token_parser()
// 		: token_parser::base_type(r_start)
// 	{
// 
// 		//
// 		//	r_literal will be converted from string to object automatically
// 		//
// 		r_start
// 			%= (r_obj | r_literal) % boost::spirit::omit[+boost::spirit::ascii::blank]
// 			;
// 			
// 		r_literal
// 			%= *(boost::spirit::qi::alnum | boost::spirit::qi::ascii::char_("_."))
// 			;
// 
// 	}
		
} 	//	cyng

#endif	// CYNG_PARSER_TOKEN_HPP


