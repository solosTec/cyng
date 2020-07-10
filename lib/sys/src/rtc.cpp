/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/rtc.h>
#include <cyng/chrono.h>

#include <iostream>

#include <boost/predef.h>
#include <boost/assert.hpp>

#if BOOST_OS_WINDOWS

//#include <cyng/scm/win_registry.h>

#elif BOOST_OS_LINUX

#include <linux/rtc.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#else
#warning unknow OS
#endif

namespace cyng 
{
	namespace sys
	{
#if BOOST_OS_LINUX
		std::pair<std::chrono::system_clock::time_point, bool> read_hw_clock(std::size_t idx)
		{
            std::string const device = "/dev/rtc" + std::to_string(idx);
			int fd = ::open(device.c_str(), 0);
			if (fd < 0)	return std::make_pair(std::chrono::system_clock::now(), false);

			struct rtc_time time;
			if (::ioctl(fd, RTC_RD_TIME, &time) < 0)	return std::make_pair(std::chrono::system_clock::now(), false);

			return std::make_pair(chrono::init_tp(time.tm_year + 1900
				, time.tm_mon + 1
				, time.tm_mday
				, time.tm_hour
				, time.tm_min
				, time.tm_sec), true);	//seconds
		}

		bool write_hw_clock(std::chrono::system_clock::time_point tp, std::size_t idx)
		{
			//	CAP_SYS_TIME privilege required
            std::string const device = "/dev/rtc" + std::to_string(idx);
			int fd = ::open(device.c_str(), 0);
			if (fd < 0)	return false;

			auto const tm = chrono::make_utc_tm(tp);

			struct rtc_time time;

			time.tm_sec = tm.tm_sec;
			time.tm_min = tm.tm_min;
			time.tm_hour = tm.tm_hour;
			time.tm_mday = tm.tm_mday;
			time.tm_mon = tm.tm_mon;
			time.tm_year = tm.tm_year;
            time.tm_wday	= 0;  // days since Sunday - [0, 6]
			time.tm_yday	= 0;  // days since January 1 - [0, 365]
			time.tm_isdst	= -1; // daylight savings time flag [-1/0/1]
			
			return !(::ioctl(fd, RTC_SET_TIME, &time) < 0);
	}
#endif


#if BOOST_OS_WINDOWS
#endif

	}
}
