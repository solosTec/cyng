/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_JSON_PARSER_HPP
#define CYNG_JSON_PARSER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/json/json_parser.h>
#include <cyng/factory.h>
#ifdef _DEBUG
#include <cyng/parser/spirit_support.hpp>
#endif

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyng::object

namespace boost		
{ 
	namespace spirit	
	{ 
		namespace traits	
		{ 
	
			template <> 
			struct transform_attribute< cyng::object, cyng::tuple_t, boost::spirit::qi::domain > 
			{ 
				typedef cyng::tuple_t type; 
				static cyng::tuple_t pre(cyng::object& v) 
				{ 
					return cyng::tuple_t(); 
				} 
				static void post(cyng::object& val, cyng::tuple_t const& attr) 
				{
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, cyng::vector_t, boost::spirit::qi::domain > 
			{ 
				typedef cyng::vector_t type; 
				static cyng::vector_t pre(cyng::object& v) 
				{ 
					return cyng::vector_t(); 
					
				} 
				static void post(cyng::object& val, cyng::vector_t const& attr) 
				{
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

		}	//	traits
	}	//	spirit
}	//	boost
	
namespace cyng	
{
	namespace
	{

		//	Transform an Attribute to a Different Type 
		//	boost sprits transform_attribute<> template would be another
		//	solution.
		struct cast_from_member_impl
		{
			template < typename A >
			struct result { typedef object type; };

			object operator()(boost::fusion::vector < utf::u32_string, object > arg) const
			{
				const utf::u32_string utf8_key = boost::fusion::front(arg);
				std::string key = utf::u32_to_u8_string(utf8_key);

				const object value = boost::fusion::back(arg);
				return make_object(param_t(key, value));
			}
		};

		/**
		 * helper function to create an object of type UUID from a buffer
		 * of size 16.
		 */
		object uuid_from_buffer_factory(buffer_t const& b)
		{
			BOOST_ASSERT_MSG(b.size() == 16, "wrong buffer size");
			if (b.size() == 16)
			{
				boost::uuids::uuid v;
				//	C4996
				std::copy(b.begin(), b.end(), std::begin(v.data));
				return make_object(v);
			}
			return make_object();
		}
	}	//	namespace *anonymous*

	template <typename Iterator, typename Skipper>
	json_parser< Iterator, Skipper > :: json_parser()
		: json_parser::base_type( r_json )
	{
		boost::phoenix::function<cast_from_member_impl>	cast_from_member;
		
		r_json
			= r_obj[boost::spirit::_val = boost::spirit::_1]
			;
			
		r_obj
			%= r_uuid_obj	//	auto detect UUIDs
			| r_numeric		//	int, uint, double
			| r_string		//	already object
			| (boost::spirit::qi::lit('@') > r_dt)[boost::spirit::_val = boost::spirit::_1]
			| boost::spirit::qi::lit("true")[boost::spirit::_val = make_object(true)]
			| boost::spirit::qi::lit("false")[boost::spirit::_val = make_object(false)]
			| boost::spirit::qi::lit("null")[boost::spirit::_val = make_object()]
			| ('[' > (r_vector | boost::spirit::eps[boost::spirit::_val = make_vector()])> ']')	//	automatic conversion 
			| ('{' > (r_members | boost::spirit::eps[boost::spirit::_val = make_tuple()]) > '}')	//	automatic conversion 
			;
		
		r_members
			%= p_ % ','
			;
		
		//	needed as conversion step from "r_pair" to "object"
		p_
			= r_pair[boost::spirit::_val = cast_from_member(boost::spirit::_1)]
			;

		r_pair
			= (r_quote | r_ident ) >> boost::spirit::qi::lit(':') >> r_obj.alias()
			;

		r_value
			= r_obj[boost::spirit::_val = boost::spirit::_1]
			;

		//
		//	no type check. it's possible to populate
		//	the vector with values of different data types.
		//
		r_vector
			= r_value % ','
			;
		
 		r_uuid_obj
 			= r_uuid[boost::spirit::_val = boost::phoenix::bind(&uuid_from_buffer_factory, boost::spirit::_1)]
 			;
 
		r_uuid
			= '"'
			>> boost::spirit::qi::repeat(4)[r_hex2] >> '-'
			>> boost::spirit::qi::repeat(2)[r_hex2] >> '-'
			>> boost::spirit::qi::repeat(2)[r_hex2] >> '-'
			>> boost::spirit::qi::repeat(2)[r_hex2] >> '-'
			>> boost::spirit::qi::repeat(6)[r_hex2]
			>> '"'
			;

		r_json.name("json_parser");
		
	}
} 	//	cyng

#endif	// CYNG_JSON_PARSER_HPP


