/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/chrono.h>
#include <ctime>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <boost/assert.hpp>
#include <boost/core/ignore_unused.hpp>

namespace cyng 
{
	namespace chrono 
	{		
		int year(std::tm const& t)
		{
			return t.tm_year + 1900;
		}
		
		int month(std::tm const& t)
		{
			return t.tm_mon + 1;
		}
		
		int day(std::tm const& t)
		{
			return t.tm_mday;
		}
		
		int day_of_year(std::tm const& t)
		{
			return t.tm_yday;
		}

		int hour(std::tm const& t)
		{
			return t.tm_hour;
		}

		int minute(std::tm const& t)
		{
			return t.tm_min;
		}

		int second(std::tm const& t)
		{
			return t.tm_sec;
		}

		int time_of_day(std::tm const& t)	{
			
			BOOST_ASSERT_MSG(t.tm_mon > 0 && t.tm_mon < 13, "month is out of range");
			BOOST_ASSERT_MSG(t.tm_mday >= 0 && t.tm_mday < 32, "day is out of range");
			
			return t.tm_sec + (t.tm_min * 60) + (t.tm_hour * 60 * 60);	
        }
        
        std::tm init_tm(int year, int month, int day, int hour, int min, int sec)
        {
            BOOST_ASSERT_MSG(year > 1900, "year is out of range");
			BOOST_ASSERT_MSG(month > 0, "month is out of range");
#ifdef _MSC_VER
			//	MSC does not support aggregate initialisation of struct
			std::tm t;
			t.tm_sec	= sec;	//	tm_sec - Seconds.	[0-60] (1 leap second)
			t.tm_min	= min;	//	tm_min - Minutes.	[0-59]
			t.tm_hour	= hour;	//	tm_hour - Hours.	[0-23]
			t.tm_mday	= day;	//	tm_mday -  Day.		[1-31]
			t.tm_mon	= month - 1;	//	tm_mon - Month.	[0-11]
			t.tm_year	= year - 1900;	//	tm_year - Year	- 1900
			t.tm_wday	= 0;  // days since Sunday - [0, 6]
			t.tm_yday	= 0;  // days since January 1 - [0, 365]
			t.tm_isdst	= -1; // daylight savings time flag [-1/0/1]
			return t;

#else
			return std::tm {
			    sec,	//	tm_sec - Seconds.	[0-60] (1 leap second)
			    min,	//	tm_min - Minutes.	[0-59]
			    hour,	//	tm_hour - Hours.	[0-23]
			    day,	//	tm_mday -  Day.		[1-31]
			    month - 1,		//	tm_mon - Month.	[0-11]
 			    year - 1900,	//	tm_year - Year	- 1900
			    //	unused
			    0,		//	tm_wday - Day of week  [0-6]
			    0,		//	tm_yday - Days in year [0-365]
			    -1,		//	tm_isdst - DST. [-1/0/1]
				0, 		//  Seconds east of UTC. 
				""		//  Timezone abbreviation. 
            };
#endif
        }

		std::time_t tm_to_tt(std::tm const& t)
		{
			std::tm tmp = t;
#ifdef BOOST_WINDOWS
			return ::_mkgmtime(&tmp);
#else
			//	nonstandard GNU extension, also present on the BSDs
			return ::timegm(&tmp);
#endif
		}
        
        std::chrono::system_clock::time_point init_tp(std::uint16_t year, std::uint8_t month, std::uint8_t day, std::uint8_t hour, std::uint8_t min, double sec)
        {
            const std::time_t tt = tm_to_tt(init_tm(year, month, day, hour, min, 0));
			std::chrono::duration<double> d(sec);
			return std::chrono::system_clock::from_time_t(tt) + std::chrono::duration_cast<std::chrono::microseconds>(d);            
        } 
        
		std::tm convert_utc(std::time_t tt)
		{
			std::tm r{ 0 };
			
#ifdef BOOST_WINDOWS
			const errno_t e = ::gmtime_s(&r, &tt);
			if (e != 0)
			{
				char msg[128];
				::strerror_s(msg, 128, e);
				std::cerr << msg << std::endl;
			}
			BOOST_ASSERT_MSG (e == 0, "convert_utc");
#else
			//	POSIX API
			const struct tm* ptr = ::gmtime_r(&tt, &r);
			BOOST_ASSERT_MSG(ptr != nullptr, "convert_utc");
			boost::ignore_unused(ptr);			
#endif
			return r;
		}
		
		std::tm convert_local(std::time_t tt)
		{
#ifdef _MSC_VER
			struct tm r;
			::localtime_s(&r, &tt);
			return r;
#else
			return *::localtime( &tt );
#endif
// 			BOOST_ASSERT_MSG(ptr != nullptr, "convert_local";
		}
        
		std::chrono::minutes delta()
		{
			//
			//	get a time point
			//
			const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
			const std::time_t now_tt = std::chrono::system_clock::to_time_t(now);
			
			//
			//	get UTC and local time
			//
			const std::tm utc_tt = convert_utc(now_tt);
			const std::tm local_tt = convert_local(now_tt);
			
			//
			//	calculate offset
			//
			return std::chrono::minutes(60 * (utc_tt.tm_hour - local_tt.tm_hour) + (utc_tt.tm_min - local_tt.tm_min));
		}
        
		std::pair<std::time_t, double> to_dbl_time_point(std::chrono::system_clock::time_point const& tp)
		{
			//	Probably precision get lost.
			const std::time_t tt = std::chrono::system_clock::to_time_t(tp);
			dbl_seconds sec  = tp - std::chrono::system_clock::from_time_t(tt);
			BOOST_ASSERT_MSG(sec.count() < 1.0, "error in fractional second");
			return std::make_pair(tt, sec.count());
		}

		std::chrono::system_clock::time_point to_time_point(dbl_time_point const& dtp)
		{
			//	Probably precision get lost.
			return std::chrono::system_clock::from_time_t(dtp.first)
				+ std::chrono::duration_cast<std::chrono::microseconds>(dbl_seconds(dtp.second));
		}

                
		std::chrono::system_clock::duration duration_of_day(std::chrono::system_clock::time_point const& tp)
		{
			std::chrono::system_clock::duration d = tp.time_since_epoch();
			std::chrono::hours h = std::chrono::duration_cast<std::chrono::hours>(d);
			d -= std::chrono::hours(h.count() % 24);
			std::chrono::minutes m = std::chrono::duration_cast<std::chrono::minutes>(d);
			d -= std::chrono::minutes(m.count() % 60);
			std::chrono::seconds s = std::chrono::duration_cast<std::chrono::seconds>(d);
			d -= std::chrono::seconds(s.count() % 60);
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(d);
			d -= std::chrono::milliseconds(ms.count() % 1000);
			return tp.time_since_epoch() - d;
		}

		std::chrono::system_clock::time_point strip_time(std::chrono::system_clock::time_point const& tp)
		{
			return (tp - duration_of_day(tp));
		}
		
		std::string to_string(std::chrono::system_clock::time_point const& tp)
		{
			const std::time_t tt = std::chrono::system_clock::to_time_t(tp);
#ifdef _MSC_VER
			char str[26];
			::ctime_s(str, sizeof str, &tt);
			std::string s(str, sizeof str);
#else
			std::string s = std::ctime(&tt);
#endif
			return s.substr(0, s.size() - 1);	//	remove NL
		}
		
		std::tm make_utc_tm(std::chrono::system_clock::time_point tp)
		{
			return convert_utc(std::chrono::system_clock::to_time_t(tp));
		}

		std::chrono::system_clock::time_point add_month(std::chrono::system_clock::time_point const& tp, int month)
		{
			auto tt = std::chrono::system_clock::to_time_t(tp);

			//
			//	calulate lost accuracy
			//
			dbl_seconds sec = tp - std::chrono::system_clock::from_time_t(tt);
			auto tm = convert_utc(tt);

			//
			//	add/sub one or multiple months.
			//
			while (month > 0) {
				if (tm.tm_mon == 11) {
					tm.tm_mon = 0;
					tm.tm_year++;
				}
				else {
					tm.tm_mon++;
				}
				month--;
			}
			while (month < 0) {
				if (tm.tm_mon == 0) {
					tm.tm_mon = 11;
					tm.tm_year--;
					BOOST_ASSERT_MSG(tm.tm_year > 1900, "year is out of range");
				}
				else {
					tm.tm_mon--;
				}
				month++;
			}
			return std::chrono::system_clock::from_time_t(tm_to_tt(tm))
				+ std::chrono::duration_cast<std::chrono::microseconds>(dbl_seconds(sec));
		}

		days days_of_month(std::chrono::system_clock::time_point tp)
		{
			auto tm = make_utc_tm(tp);

			auto begin = cyng::chrono::init_tp(cyng::chrono::year(tm)
				, cyng::chrono::month(tm)
				, 1 //	1. day
				, 0	//	hour
				, 0	//	minute
				, 0.0); //	this day

			//
			//	begin of next month
			//
			tp = add_month(tp, 1);
			tm = make_utc_tm(tp);
			auto end = cyng::chrono::init_tp(cyng::chrono::year(tm)
				, cyng::chrono::month(tm)
				, 1 //	1. day
				, 0	//	hour
				, 0	//	minute
				, 0.0); //	this day

			return std::chrono::duration_cast<days>(end - begin);
		}

		bool same_day(std::chrono::system_clock::time_point tp1, std::chrono::system_clock::time_point tp2)
		{
			const auto tm1 = make_utc_tm(tp1);
			const auto tm2 = make_utc_tm(tp2);
			return (year(tm1) == year(tm2)) && (day_of_year(tm1) == day_of_year(tm2));
		}

	}
}


