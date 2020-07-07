/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SYS_RTC_H
#define CYNG_SYS_RTC_H

#include <chrono>
#include <string>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * Read and write hardware clock.
		 * CAP_SYS_TIME privilege required.
		 * These functions are not available on windows.
		 */
#if BOOST_OS_LINUX
		std::chrono::system_clock::time_point read_hw_clock();
		bool write_hw_clock(std::chrono::system_clock::time_point);
#endif
	}
}

#endif
