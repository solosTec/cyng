/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_PARSER_CHRONO_HPP
#define CYNG_PARSER_CHRONO_HPP

#include <cyng/parser/chrono_parser.h>
#include <cyng/factory.h>

#include <boost/spirit/home/support/attributes.hpp>	//	transform_attribute
#include <boost/spirit/include/phoenix.hpp>	//	enable assignment of values like cyng::object
	
namespace cyng	
{
	namespace	
	{

		//	Transform an Attribute to a Different Type 
		//	boost sprits transform_attribute<> template would be another
		//	solution.
		struct tp_intrinsic_factory
		{
			template < typename A >
			struct result { typedef object type; };

			object operator()(const boost::fusion::vector<std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, double>& arg) const
			{
				auto sec = boost::fusion::at_c< 5 >(arg);		// fractal seconds
				auto minute = boost::fusion::at_c< 4 >(arg);
				auto hour = boost::fusion::at_c< 3 >(arg);
				auto day = boost::fusion::at_c< 2 >(arg);
				auto month = boost::fusion::at_c< 1 >(arg);
				auto year = boost::fusion::at_c< 0 >(arg);
				
				std::chrono::system_clock::time_point tp = chrono::init_tp(year, month, day, hour, minute, sec);
				return make_object(tp);
			}
		};
		
		struct tp_sql_factory
		{
			template < typename A >
			struct result { typedef std::chrono::system_clock::time_point type; };

			std::chrono::system_clock::time_point operator()(const boost::fusion::vector<std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>& arg) const
			{
				//	"2015-11-28 11:06:44"
				auto sec = boost::fusion::at_c< 5 >(arg);
				auto minute = boost::fusion::at_c< 4 >(arg);
				auto hour = boost::fusion::at_c< 3 >(arg);
				auto day = boost::fusion::at_c< 2 >(arg);
				auto month = boost::fusion::at_c< 1 >(arg);
				auto year = boost::fusion::at_c< 0 >(arg);
				
				return chrono::init_tp(year, month, day, hour, minute, sec);
// 				return cyng::chrono::create(year, month, day, hour, minute, sec);
			}
		};
		
		struct tp_var_factory
		{
			template < typename A >
			struct result { typedef std::chrono::system_clock::time_point type; };

			std::chrono::system_clock::time_point operator()(const boost::fusion::vector<std::uint8_t, std::uint8_t, std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t>& arg) const
			{
				//	"30.09.2016 13:34:26"
				auto sec = boost::fusion::at_c< 5 >(arg);
				auto minute = boost::fusion::at_c< 4 >(arg);
				auto hour = boost::fusion::at_c< 3 >(arg);
				auto day = boost::fusion::at_c< 0 >(arg);
				auto month = boost::fusion::at_c< 1 >(arg);
				auto year = boost::fusion::at_c< 2 >(arg);

				return chrono::init_tp(year, month, day, hour, minute, sec);
// 				return cyng::chrono::create(year, month, day, hour, minute, sec);
			}
		};

		struct tp_json_factory
		{
			template < typename A >
			struct result { typedef std::chrono::system_clock::time_point type; };

			std::chrono::system_clock::time_point operator()(const boost::fusion::vector<std::uint16_t, std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t, double, int>& arg) const
			{
				auto offset = boost::fusion::at_c< 6 >(arg);	//	int
				auto sec = boost::fusion::at_c< 5 >(arg);	//	double
				auto minute = boost::fusion::at_c< 4 >(arg);
				auto hour = boost::fusion::at_c< 3 >(arg);
				auto day = boost::fusion::at_c< 2 >(arg);
				auto month = boost::fusion::at_c< 1 >(arg);
				auto year = boost::fusion::at_c< 0 >(arg);

				return chrono::init_tp(year, month, day, hour, minute, sec);
// 				return cyng::chrono::create(year, month, day, hour, minute + offset, sec);
			}
		};

		struct time_span_factory
		{
			template < typename A >
			struct result { typedef std::chrono::microseconds type; };

			std::chrono::microseconds operator()(const boost::fusion::vector<std::uint8_t, std::uint8_t, double>& arg) const
			{
				auto secs = boost::fusion::at_c< 2 >(arg);
				auto minutes = boost::fusion::at_c< 1 >(arg);
				auto hours = boost::fusion::at_c< 0 >(arg);
				
				//return cyng::chrono::create(year, month, day, hour, minute, sec);
				std::chrono::microseconds::rep count = (secs * 1000ULL * 1000ULL)
				+ (minutes * 60ULL * 1000ULL * 1000ULL)
				+ (hours * 60ULL * 60ULL * 1000ULL * 1000ULL)
				;
				return std::chrono::microseconds(count);
			}
		};
		
	}

	template <typename Iterator, typename Skipper>
	chrono_parser< Iterator, Skipper > :: chrono_parser()
		: chrono_parser::base_type( r_start )
	{

		boost::phoenix::function<tp_intrinsic_factory>	make_intrinsic_tp;
		
		r_start
		%= boost::spirit::qi::lit("now")[boost::spirit::_val = make_now()]
		| boost::spirit::qi::lit("\"min\"ts")[boost::spirit::_val = make_time_point_min()]
		| boost::spirit::qi::lit("\"max\"ts")[boost::spirit::_val = make_time_point_max()]
		| r_nanosecond
		| r_microsecond
		| r_millisecond
		| r_second
		| r_minute
		| r_hour
		| r_day
		| '"' >> r_tp >> "\"chrono:tp"
		| '@' >> r_rfc3339
		;
		
		r_nanosecond
		= (r_uint64 >> "ns")[boost::spirit::_val = boost::phoenix::bind(&make_nanoseconds, boost::spirit::_1)]
		;
		
		r_microsecond
		= (r_uint64 >> "us")[boost::spirit::_val = boost::phoenix::bind(&make_microseconds, boost::spirit::_1)]
		;
		
		r_millisecond
		= (r_uint64 >> "ms")[boost::spirit::_val = boost::phoenix::bind(&make_milliseconds, boost::spirit::_1)]
		;
		
		r_second
		= (r_uint64 >> "chrono:sec")[boost::spirit::_val = boost::phoenix::bind(&make_seconds, boost::spirit::_1)]
		;
		
		r_minute
		= (r_uint64 >> "min")[boost::spirit::_val = boost::phoenix::bind(&make_minutes, boost::spirit::_1)]
		;
		
		r_hour
		= (r_uint64 >> "h")[boost::spirit::_val = boost::phoenix::bind(&make_hours, boost::spirit::_1)]
		;
		
		//	"d" is colliding with [d]ouble
		r_day
		= (r_uint64 >> (boost::spirit::qi::lit("days") | boost::spirit::qi::lit("day")))[boost::spirit::_val = boost::phoenix::bind(&make_days, boost::spirit::_1)]
		;
		
	}
	
	template <typename Iterator, typename Skipper>
	timepoint_basic_parser< Iterator, Skipper > ::timepoint_basic_parser()
		: timepoint_basic_parser::base_type(r_start)
	{
		boost::phoenix::function<tp_intrinsic_factory>	make_intrinsic_tp;

		r_start
			= r_time[boost::spirit::_val = make_intrinsic_tp(boost::spirit::_1)]
			;

		//	2018-04-24 08:29:9.5853489
		//	2018-08-13 18:37:08
		r_time
			%= (r_uint16 
				>> boost::spirit::qi::lit(L'-') 
				>> r_uint8 
				>> boost::spirit::qi::lit(L'-')
				>> r_uint8
				>> boost::spirit::qi::lit(L' ')
				>> r_uint8
				>> boost::spirit::qi::lit(L':')
				>> r_uint8
				>> boost::spirit::qi::lit(L':')
				>> boost::spirit::qi::double_)
			;
	}

	template <typename Iterator>
	db_timestamp_parser< Iterator > :: db_timestamp_parser()
		: db_timestamp_parser::base_type( r_start )
	{

		boost::phoenix::function<tp_sql_factory>	make_sql_tp;
		boost::phoenix::function<tp_var_factory>	make_var_tp;

		r_start
			= r_time_1[boost::spirit::_val = make_sql_tp(boost::spirit::_1)]
			| r_time_2[boost::spirit::_val = make_var_tp(boost::spirit::_1)]
			;
		
		//	"2015-11-28 11:06:44"
		r_time_1
			%= (r_uint16 >> '-' >> r_uint8 >> '-' >> r_uint8 >> ' ' >> r_uint8 >> ':'  >> r_uint8 >> ':' >> r_uint8)
			;

		//	"30.09.2016 13:34:26"
		r_time_2
			%= (r_uint8 >> '.' >> r_uint8 >> '.' >> r_uint16 >> ' ' >> r_uint8 >> ':' >> r_uint8 >> ':' >> r_uint8)
			;
	}
	
	template <typename Iterator>
	timespan_parser< Iterator >::timespan_parser()
		: timespan_parser::base_type(r_start)
	{
		boost::phoenix::function<time_span_factory>	make_time_span;

		r_start
			= boost::spirit::qi::lit("m:a:x")[boost::spirit::_val = std::chrono::microseconds::max()]
			| boost::spirit::qi::lit("m:i:n")[boost::spirit::_val = std::chrono::microseconds::min()]
			| boost::spirit::qi::lit("n:i:l")[boost::spirit::_val = std::chrono::microseconds::zero()]
			| r_span[boost::spirit::_val = make_time_span(boost::spirit::_1)]
			;

		r_span
			%= r_uint8	//	hour
			>> ':'
			>> r_uint8	//	min
			>> ':'
			>> boost::spirit::qi::double_	//	sec
			;
	}
	
	template <typename Iterator>
	rfc3339_timestamp_parser< Iterator > ::rfc3339_timestamp_parser()
		: rfc3339_timestamp_parser::base_type(r_start)
	{

		boost::phoenix::function<tp_json_factory>	make_json_tp;

		r_start
			= r_time[boost::spirit::_val = make_json_tp(boost::spirit::_1)]
			;

			//	2022-10-02T15:00:00.05Z
			//	1937-01-01T12:00:27.87+00:20
			//	1996-12-19T16:39:57-08:00
		r_time
			%= (r_uint16 >> '-' >> r_uint8 >> '-' >> r_uint8 >> 'T' >> r_uint8 >> ':' >> r_uint8 >> ':' >> boost::spirit::qi::double_ >> r_offset)
			;

		//	ToDo: get complete offset in minutes
		r_offset
			= boost::spirit::qi::lit("Z")[boost::spirit::_val = 0]
			| boost::spirit::qi::lit("UTC")[boost::spirit::_val = 0]
			| (boost::spirit::qi::lit("+") >> r_span)[boost::spirit::_val = boost::spirit::_1]
			| (boost::spirit::qi::lit("-") >> r_span)[boost::spirit::_val = boost::spirit::_1 * -1]
			;

		r_span
			= (boost::spirit::qi::int_ 	//	hour
			>> ':'
			>> boost::spirit::qi::int_) 	//	min
			[boost::spirit::_val = boost::spirit::_1 + (boost::spirit::_2 * 60)]
			;
	}

	template <typename Iterator>
	rfc3339_obj_parser< Iterator > ::rfc3339_obj_parser()
		: rfc3339_obj_parser::base_type(r_start)
	{
		r_start
			= r_ts[boost::spirit::_val = boost::phoenix::bind(&rfc3339_obj_parser<Iterator>::helper, this, boost::spirit::_1)]
			;
	}

	template <typename Iterator>
	object rfc3339_obj_parser< Iterator > ::helper(std::chrono::system_clock::time_point const& tp)
	{
		return make_object(tp);
	}

	
} 	//	cyng

#endif	// CYNG_PARSER_CHRONO_HPP


