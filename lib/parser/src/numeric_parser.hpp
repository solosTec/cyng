/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_NUMERIC_HPP
#define CYNG_PARSER_NUMERIC_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/parser/numeric_parser.h>
#include <cyng/parser/spirit_support.hpp>
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
			struct transform_attribute< cyng::object, std::uint8_t, boost::spirit::qi::domain > 
			{ 
				typedef std::uint8_t type; 
				static std::uint8_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::uint8_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, std::uint16_t, boost::spirit::qi::domain > 
			{ 
				typedef std::uint16_t type; 
				static std::uint16_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::uint16_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, std::uint32_t, boost::spirit::qi::domain > 
			{ 
				typedef std::uint32_t type; 
				static std::uint32_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::uint32_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, std::uint64_t, boost::spirit::qi::domain > 
			{ 
				typedef std::uint64_t type; 
				static std::uint64_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::uint64_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, std::int8_t, boost::spirit::qi::domain > 
			{ 
				typedef std::int8_t type; 
				static std::int8_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::int8_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, std::int16_t, boost::spirit::qi::domain > 
			{ 
				typedef std::int16_t type; 
				static std::int16_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::int16_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, std::int32_t, boost::spirit::qi::domain > 
			{ 
				typedef std::int32_t type; 
				static std::int32_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::int32_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, std::int64_t, boost::spirit::qi::domain > 
			{ 
				typedef std::int64_t type; 
				static std::int64_t pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, std::int64_t const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, float, boost::spirit::qi::domain > 
			{ 
				typedef float type; 
				static float pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, float const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, double, boost::spirit::qi::domain > 
			{ 
				typedef double type; 
				static double pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, double const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

			template <> 
			struct transform_attribute< cyng::object, long double, boost::spirit::qi::domain > 
			{ 
				typedef long double type; 
				static long double pre(cyng::object& v) { return 0; } 
				static void post(cyng::object& val, long double const& attr) {
					val = cyng::make_object(attr);
				} 
				static void fail(cyng::object&) {}
			}; 

		}	//	traits
	}	//	spirit
}	//	boost
	
namespace cyng	
{

	/**
	 * To convert the results from the numeric parser
	 * to objects the customization points above are called
	 * by boost::spirit::attr_cast<>().
	 */
	template <typename Iterator>
	numeric_parser< Iterator > :: numeric_parser()
		: numeric_parser::base_type( r_start )
	{
		r_start
		%= boost::spirit::qi::lit("true")[boost::spirit::_val = make_object(true)]
		| boost::spirit::qi::lit("false")[boost::spirit::_val = make_object(false)]
		| boost::spirit::attr_cast<object, std::uint8_t>(r_uint8) >> "u8"
		| boost::spirit::attr_cast<object, std::uint16_t>(r_uint16) >> "u16"
		| boost::spirit::attr_cast<object, std::uint32_t>(r_uint32) >> "u32"
		| boost::spirit::attr_cast<object, std::uint64_t>(r_uint64) >> "u64"
		| boost::spirit::attr_cast<object, std::uint64_t>(r_uint64) >> "u"
		| boost::spirit::attr_cast<object, std::int8_t>(r_int8) >> "i8"
		| boost::spirit::attr_cast<object, std::int16_t>(r_int16) >> "i16"
		| boost::spirit::attr_cast<object, std::int32_t>(r_int32) >> "i32"
		| boost::spirit::attr_cast<object, std::int64_t>(r_int64) >> "i64"
		| boost::spirit::attr_cast<object, std::int64_t>(r_int64) >> "i"
		| boost::spirit::attr_cast<object, float>(boost::spirit::qi::float_) >> "f"
		| boost::spirit::attr_cast<object, double>(boost::spirit::qi::double_) >> "d"
		| boost::spirit::attr_cast<object, long double>(boost::spirit::qi::long_double) >> "ld"
	
		| boost::spirit::ascii::no_case["0x"] >> boost::spirit::qi::attr_cast(boost::spirit::qi::hex)
		| "0" >> boost::spirit::qi::attr_cast(boost::spirit::qi::oct)	//	trailing 0 for octal values
		| boost::spirit::qi::attr_cast(r_strict_double)	//	without a postfix we assume double
		| r_int	//	without a postfix we assume signed int
		;
			
		r_int
		%= boost::spirit::attr_cast<object, std::int32_t>(r_int32)
		;
		
		r_start.name("numeric_parser");
		r_int.name("r_int");
		//debug(r_start);
		//debug(r_int);
	}
} 	//	cyng

#endif	// CYNG_PARSER_NUMERIC_HPP


