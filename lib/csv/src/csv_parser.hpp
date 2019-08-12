/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CSV_PARSER_HPP
#define CYNG_CSV_PARSER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <cyng/csv/csv_parser.h>
#include <cyng/factory.h>
#ifdef _DEBUG
#include <cyng/parser/spirit_support.hpp>
#endif

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyng::object

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
	csv_parser< Iterator, Skipper > :: csv_parser()
		: csv_parser::base_type( r_csv )
	{
		boost::phoenix::function<cast_from_member_impl>	cast_from_member;
		
		r_csv
			= r_line[boost::spirit::_val = boost::spirit::_1]
			;
			
		r_line
			%= r_value % ','
			;

		r_value
			%= r_uuid_obj	//	auto detect UUIDs
			| r_numeric		//	int, uint, double
			| r_string		//	already object
			| (boost::spirit::qi::lit('@') > r_dt)[boost::spirit::_val = boost::spirit::_1]
			| boost::spirit::qi::lit("true")[boost::spirit::_val = make_object(true)]
			| boost::spirit::qi::lit("false")[boost::spirit::_val = make_object(false)]
			| boost::spirit::qi::lit("null")[boost::spirit::_val = make_object()]
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

		r_csv.name("csv_parser");
		
	}
} 	//	cyng

#endif	// CYNG_csv_PARSER_HPP


