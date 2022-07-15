#include <cyng/sys/clock.h>

//	From Howard Hinnants data library.
//	Used here since not all compilers C++20 compliant yet
#include <date/iso_week.h>
#include <date/date.h>

#include <boost/predef.h>
#include <boost/assert.hpp>

#include <iostream>


namespace cyng
{
	namespace sys
	{
		std::chrono::system_clock::time_point get_start_of_day(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			return date::year_month_day{ this_day }.operator std::chrono::sys_days();
		}

		std::chrono::system_clock::time_point get_end_of_day(std::chrono::system_clock::time_point tp) {
			return get_start_of_day(tp) + std::chrono::hours(24);
		}

		std::chrono::system_clock::time_point get_start_of_month(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return date::year_month_day{ ymd.year(), ymd.month(), date::day{ 1 } }.operator std::chrono::sys_days();
		}
		std::chrono::system_clock::time_point get_end_of_month(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return date::year_month_day_last{ ymd.year(), date::month_day_last{ ymd.month()} }.operator std::chrono::sys_days();
		}

		std::uint64_t get_length_of_month(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return  date::year_month_day_last{ ymd.year(), date::month_day_last{ ymd.month()} }.day().operator unsigned int();
		}

		std::chrono::system_clock::time_point get_end_of_year(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return  date::year_month_day_last{ ymd.year(), date::month_day_last{ date::December } }.operator std::chrono::sys_days();
		}

		std::chrono::system_clock::time_point get_start_of_year(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return date::year_month_day{ ymd.year(), date::January, date::day{ 1 } }.operator std::chrono::sys_days();
		}

		std::uint64_t get_length_of_year(std::chrono::system_clock::time_point tp) {
			auto const d = std::chrono::duration_cast<date::days>(get_end_of_year(tp) - get_start_of_year(tp));
			return d.count();
		}

		std::uint64_t get_iso_week_number(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			auto const this_day_iso = iso_week::year_weeknum_weekday{ this_day };

			//	2022-W27-Sun
			//std::cout << this_day_iso << std::endl;  // 2016-W44-Mon
			return this_day_iso.weeknum().operator unsigned int();
		}

		std::uint64_t get_day_of_week(std::chrono::system_clock::time_point tp) {
			auto const this_day = date::floor<date::days>(tp);
			auto const this_day_iso = iso_week::year_weeknum_weekday{ this_day };
			return this_day_iso.weekday().operator unsigned int();
		}


	}
}
