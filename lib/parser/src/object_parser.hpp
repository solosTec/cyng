/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_OBJECT_HPP
#define CYNG_PARSER_OBJECT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/parser/object_parser.h>
#include <cyng/parser/spirit_support.hpp>
#include <cyng/intrinsics.h>
#include <cyng/factory.h>
#include <cyng/value_cast.hpp>

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyng::object

namespace boost		
{ 
	namespace spirit	
	{ 
		namespace traits	
		{ 
			/**
			 * Convert a tuple into an object.
			 */
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
				static void fail(cyng::object&) 
				{}
			}; 

			/**
			 * Define a customization point with transform_attribute<>.
			 * Create an cyng attribute.
			 * 
			 * @see http://boost-spirit.com/home/2010/02/08/how-to-adapt-templates-as-a-fusion-sequence/
			 */
			template <>
			struct transform_attribute< cyng::object, boost::fusion::vector < std::size_t, cyng::object >, boost::spirit::qi::domain > 
			{ 
				typedef boost::fusion::vector < std::size_t, cyng::object > type; 
				static type pre(cyng::object& v) { return type(0u, cyng::object()); } 
				static void post(cyng::object& obj, type const& attr) {
					
					const std::size_t index = boost::fusion::front( attr );
					const cyng::object value = boost::fusion::back( attr );
					obj = cyng::make_object(std::make_pair(index, value));
				} 
				static void fail(cyng::object&) 
				{}
			}; 

			template <>
			struct transform_attribute< cyng::object, boost::fusion::vector < cyng::utf::u32_string, cyng::object >, boost::spirit::qi::domain > 
			{ 
				typedef boost::fusion::vector < cyng::utf::u32_string, cyng::object > type; 
				static type pre(cyng::object& v) { return type(cyng::utf::u32_string(), cyng::object()); } 
				static void post(cyng::object& obj, type const& attr) {
					
					const cyng::utf::u32_string key = boost::fusion::front( attr );
					const cyng::object value = boost::fusion::back( attr );
					obj = cyng::make_object(std::make_pair("todo", value));
				} 
				static void fail(cyng::object&) 
				{}
			}; 
			
		}	//	traits
	}	//	spirit
}	//	boost
	
namespace cyng	
{
	namespace
	{

		//	Convert a vector of objects into a attribute map
		struct cast_to_amap_impl
		{
			template < typename A >
			struct result { typedef object type; };

			object operator()(cyng::vector_t vec) const
			{
				attr_map_t amap;
				attr_t def;
				for (auto obj : vec)
				{
					attr_t const& attr = value_cast(obj, def);
//						std::cout << attr.first << " := " << cyng::to_literal(attr.second) << std::endl;
					amap[attr.first] = attr.second;
				}
				return make_object(amap);
			}
		};

		struct cast_to_pmap_impl
		{
			template < typename A >
			struct result { typedef object type; };

			object operator()(cyng::vector_t vec) const
			{
				param_map_t pmap;
				param_t def;
				for (auto obj : vec)
				{
					param_t const& param = value_cast(obj, def);
//						std::cout << param.first << " := " << cyng::to_literal(param.second) << std::endl;
					pmap[param.first] = param.second;
				}
				return make_object(pmap);
			}
		};
	}	//	namespace *anonymous*

	/**
	 * To convert the results from the numeric parser
	 * to objects the customization points above are called
	 * by boost::spirit::attr_cast<>().
	 */
	template <typename Iterator, typename Skipper>
	object_parser< Iterator, Skipper > :: object_parser()
		: object_parser::base_type( r_start )
	{
		boost::phoenix::function<cast_to_amap_impl>	cast_to_amap;
		boost::phoenix::function<cast_to_pmap_impl>	cast_to_pmap;
		
		r_start
		= r_obj[boost::spirit::_val = boost::spirit::_1]
		;
		
		r_obj
		%= boost::spirit::qi::lit("null")[boost::spirit::_val = make_object()]
		| boost::spirit::qi::lit("eod")[boost::spirit::_val = make_object(eod())]
		| r_chrono
		| r_numeric	//	after r_chrono to avoid ambiguity over int_ default
		| r_quote	//	utf-8 string
		| r_buffer
		| r_version
		| r_set
		| ('{' > (r_tuple | boost::spirit::eps[boost::spirit::_val = make_object(tuple_t())]) > '}')	//	automatic conversion 
 		| ('%' > r_map)
		;
		
		//	boost::spirit::qi::standard_wide::space_type
		r_tuple
		%= r_obj % (',' >> boost::spirit::omit[*boost::spirit::standard_wide::blank])
// 		%= r_obj % (',' >> boost::spirit::omit[*boost::spirit::ascii::blank])
		;
		
		//
		//	requires a customization point
		//
 		r_set
 		= r_attr
 		| r_param
 		;
		
		//
		//	r_attr synthesizes an attribute 
		//	of type boost::fusion::vector< std::size_t, object>
		//
 		r_attr
 		= boost::spirit::qi::lit("(") 
		>> r_size 
		>> boost::spirit::qi::lit(":") 
		>> r_obj 
		>> boost::spirit::qi::lit(")")
		;
		
		//
		//	r_param synthesizes an attribute 
		//	of type boost::fusion::vector< std::size_t, object>
		//
 		r_param
 		= boost::spirit::qi::lit("(") 
		>> r_string 
		>> boost::spirit::qi::lit(":") 
		>> r_obj 
		>> boost::spirit::qi::lit(")")
		;
		
		r_map
		= ('(' >> (r_pmap)[boost::spirit::_val = cast_to_pmap(boost::spirit::_1)] >> ')')
		| ('[' >> (r_amap)[boost::spirit::_val = cast_to_amap(boost::spirit::_1)] >> ']')
		| boost::spirit::qi::lit("[]")[boost::spirit::_val = make_object(attr_map_t())]
		| boost::spirit::qi::lit("()")[boost::spirit::_val = make_object(param_map_t())]
		;

		r_amap
		%= r_attr % (',' >> boost::spirit::omit[*boost::spirit::ascii::blank])
		;
		
		r_pmap
		%= r_param % (',' >> boost::spirit::omit[*boost::spirit::ascii::blank])
		;
		
		r_start.name("object_parser");
 		r_chrono.name("chrono");
 		r_numeric.name("numeric");
	}
} 	//	cyng

#endif	// CYNG_PARSER_OBJECT_HPP



