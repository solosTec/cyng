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
#include <utility>
#include <cstddef>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * Read and write hardware clock. Each system can have multiple
		 * clocks, but most have only one.
         * 
		 * CAP_SYS_TIME privilege required.
         * 
		 * These functions are not available on windows.
		 */
#if BOOST_OS_LINUX
		std::pair<std::chrono::system_clock::time_point, bool> read_hw_clock(std::size_t idx);
		bool write_hw_clock(std::chrono::system_clock::time_point, std::size_t idx);
#endif
	}
}

#endif
