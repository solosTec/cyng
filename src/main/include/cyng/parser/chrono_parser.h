/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_CHRONO_H
#define CYNG_PARSER_CHRONO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <cyng/object.h>
#include <chrono>
#include <utility>

namespace cyng	
{
	
	/**
	 * parse a SQL timestamp
	 * "2015-11-28 11:06:44" and "30.09.2016 13:34:26"
	 */
	std::pair<std::chrono::system_clock::time_point, bool > parse_db_timestamp(std::string const&);

	/**
		* parse timespan with RFC 3339 format
		*/
	std::pair<std::chrono::system_clock::time_point, bool > parse_rfc3339_timestamp(std::string const&);

	/**
		* parse timespan with format hh:mm::ss.ffff
		*/
	std::pair<std::chrono::microseconds, bool > parse_timespan(std::string const&);

	/**
		* parse timespan with RFC 3339 format
		*/
	object parse_rfc3339_obj(std::string const&);

	/**
		* Parse date time according to RFC 3339
		* 	https://www.ietf.org/rfc/rfc3339.txt
		* Examples:
		@code
		2022-10-02T15:00:00.05Z
		1937-01-01T12:00:27.87+00:20
		1996-12-19T16:39:57-08:00
		@endcode
		*/
	template <typename Iterator>
	struct rfc3339_timestamp_parser
		: boost::spirit::qi::grammar<Iterator, std::chrono::system_clock::time_point()>
	{
		rfc3339_timestamp_parser();
		boost::spirit::qi::rule<Iterator, std::chrono::system_clock::time_point()> 	r_start;

		/**
		 * read parts of timestamp string
		 */
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 2, 4> 	r_uint16;
		boost::spirit::qi::uint_parser<std::uint8_t, 10, 1, 2>		r_uint8;
		boost::spirit::qi::rule<Iterator, boost::fusion::vector< std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, double, int>()> r_time;
		boost::spirit::qi::rule<Iterator, int()>	r_offset, r_span;

	};

	template <typename Iterator>
	struct rfc3339_obj_parser
		: boost::spirit::qi::grammar<Iterator, object()>
	{
		rfc3339_obj_parser();
		boost::spirit::qi::rule<Iterator, object()> 	r_start;
		rfc3339_timestamp_parser<Iterator>				r_ts;

		/**
			* This is a workaround since boost::phoenix::bind() cannot
			* deal with Rvalues.
			*/
		object helper(std::chrono::system_clock::time_point const&);

	};



	/**
		* Parse a raw timepoint string
		*/
	template <typename Iterator>
	struct timepoint_basic_parser
		: boost::spirit::qi::grammar<Iterator, object()>
	{
		timepoint_basic_parser();
		boost::spirit::qi::rule<Iterator, object()> 	r_start;
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 1, 4> 	r_uint16;
		boost::spirit::qi::uint_parser<std::uint8_t, 10, 1, 2>		r_uint8;
		boost::spirit::qi::rule<Iterator, boost::fusion::vector< std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, double >()> r_time;

	};

	/**
		* parse a basic timestamp like 30.09.2016 13:34:26
		*/
	std::pair<object, bool > parse_basic_timestamp(std::string const&);

	/**
		* Parse any timespan or timepoint object for a script environment. 
		*/
	template <typename Iterator>
	struct chrono_parser
	: boost::spirit::qi::grammar<Iterator, object()>
	{
		chrono_parser();
		boost::spirit::qi::rule<Iterator, object()> 	r_start;

		boost::spirit::qi::rule<Iterator, object()> r_nanosecond;
		boost::spirit::qi::rule<Iterator, object()> r_microsecond;
		boost::spirit::qi::rule<Iterator, object()> r_millisecond;
		boost::spirit::qi::rule<Iterator, object()> r_second;
		boost::spirit::qi::rule<Iterator, object()> r_minute;
		boost::spirit::qi::rule<Iterator, object()> r_hour;
		boost::spirit::qi::rule<Iterator, object()> r_day;
		
		/**
			* read tick counter of time duration
			*/
		boost::spirit::qi::uint_parser<std::uint64_t, 10, 1, -1> 	r_uint64;
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 1, 5> 	r_uint16;
		boost::spirit::qi::uint_parser<std::uint8_t, 10, 1, 3>		r_uint8;

		/**
			* Read timestamps with different formats
			*/
		timepoint_basic_parser<Iterator>	r_tp;
		rfc3339_obj_parser<Iterator>		r_rfc3339;
	};

	template <typename Iterator>
	struct db_timestamp_parser
	: boost::spirit::qi::grammar<Iterator, std::chrono::system_clock::time_point()>
	{
		db_timestamp_parser();
		boost::spirit::qi::rule<Iterator, std::chrono::system_clock::time_point()> 	r_start;

		/**
			* read parts of timestamp string
			*/
		boost::spirit::qi::uint_parser<std::uint16_t, 10, 2, 4> 	r_uint16;
		boost::spirit::qi::uint_parser<std::uint8_t, 10, 1, 2>		r_uint8;
		boost::spirit::qi::rule<Iterator, boost::fusion::vector< std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>()> r_time_1;
		boost::spirit::qi::rule<Iterator, boost::fusion::vector< std::uint8_t, std::uint8_t, std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t>()> r_time_2;

	};

	/**
		* expect the format hh:mm:ss
		*/
	template <typename Iterator>
	struct timespan_parser
	: boost::spirit::qi::grammar<Iterator, std::chrono::microseconds()>
	{
		timespan_parser();
		boost::spirit::qi::rule<Iterator, std::chrono::microseconds()> 	r_start;

		/**
			* read parts of timespan string
			*/
		boost::spirit::qi::uint_parser<std::uint8_t, 10, 1, 2>		r_uint8;
		boost::spirit::qi::rule<Iterator, boost::fusion::vector< std::uint8_t, std::uint8_t, double >()> r_span;
		
	};

} 	//	cyng

#endif	// CYNG_PARSER_CHRONO_H

