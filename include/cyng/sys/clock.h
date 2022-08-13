/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_CLOCK_H
#define CYNG_SYS_CLOCK_H

#include <string>
#include <chrono>
#include <iostream>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * @return the start time of the days relative to tp
		 */
		[[nodiscard]] std::chrono::system_clock::time_point get_start_of_day(std::chrono::system_clock::time_point tp);
		[[nodiscard]] std::chrono::system_clock::time_point get_end_of_day(std::chrono::system_clock::time_point tp);

		[[nodiscard]] std::chrono::system_clock::time_point get_start_of_month(std::chrono::system_clock::time_point);
		[[nodiscard]] std::chrono::system_clock::time_point get_end_of_month(std::chrono::system_clock::time_point);

		/**
		 * @return length of month in hours
		 */
		[[nodiscard]] std::chrono::hours get_length_of_month(std::chrono::system_clock::time_point);

		[[nodiscard]] std::chrono::system_clock::time_point get_end_of_year(std::chrono::system_clock::time_point);	
		[[nodiscard]] std::chrono::system_clock::time_point get_start_of_year(std::chrono::system_clock::time_point);

		/**
		 * @return length of year in hours
		 */
		[[nodiscard]] std::chrono::hours get_length_of_year(std::chrono::system_clock::time_point);

		/**
		 * Requires the <date/iso_week.h> header
		 * @return ISO week number of the specified time point
		 */
		//[[nodiscard]] std::uint64_t get_iso_week_number(std::chrono::system_clock::time_point);

		/**
		 * Requires the <date/iso_week.h> header
		 * @return the day of the week with Monday = 0.
		 */
		//[[nodiscard]] std::uint64_t get_day_of_week(std::chrono::system_clock::time_point);

		/**
		 * Helper function to format a time point.
		 */
		void to_string(std::ostream& os, std::chrono::system_clock::time_point const& tp, std::string format);

	}
}

#endif
