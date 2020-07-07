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
		std::chrono::system_clock::time_point read_hw_clock()
		{
			int fd = open("/dev/rtc0", 0);
			if (fd < 0)	return false;

			struct rtc_time time;
			if (ioctl(fd, RTC_RD_TIME, &time) < 0)	return std::chrono::system_clock::now();

			return chrono::init_tp(chrono::year(time)
				, chrono::month(time)
				, chrono::day(time)
				, chrono::hour(time)
				, chrono::minute(time)
				, chrono::second(time));	//seconds
		}

		bool write_hw_clock(std::chrono::system_clock::time_point tp)
		{
			//	CAP_SYS_TIME privilege required
			int fd = open("/dev/rtc0", 0);
			if (fd < 0)	return false;

			auto const tm = chrono::make_utc_tm(tp);

			struct rtc_time time;

			time.tm_sec = tm.tm_sec;
			time.tm_min = tm.tm_min;
			time.tm_hour = tm.tm_hour;
			time.tm_mday = tm.tm_mday;
			time.tm_mon = tm.tm_mon;
			time.tm_year = tm.tm_year;

			return !(ioctl(fd, RTC_SET_TIME, &time) < 0);
	}
#endif


#if BOOST_OS_WINDOWS
#endif

	}
}
