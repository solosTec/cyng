#include <cyng/sys/clock.h>

#if defined(__CYNG_USE_DATE_LIBRARY)
	//	From Howard Hinnant's awesome data library.
	//	Used here since not all compilers C++20 compliant yet
	#pragma message("include date library")
	//#include <date/iso_week.h>	requirements relaxed
	#include <date/date.h>
#else

	#if defined(__CYNG_USE_FEATURE_TESTING)
	//	feature testing is possible
		#include <version> 
		#if defined(__cpp_lib_chrono) && (__cpp_lib_chrono >= 201611L)
			#pragma message("__cpp_lib_chrono >= 201611L")
		#else
			#pragma warning("__cpp_lib_chrono < 201611L")
		#endif
	#else
		// Prior to C++20, including <ciso646> is sometimes used for this purpose.
		#include <ciso646>
	#endif

	#include <chrono>

#endif

#include <boost/predef.h>
#include <boost/assert.hpp>

#include <iostream>

namespace cyng
{
	//
	//	select namespace
	//
#if defined(CYNG_USE_DATE_LIBRARY)
	namespace chrono = date;
#else
	namespace chrono = std::chrono;
#endif

	namespace sys
	{
		std::chrono::system_clock::time_point get_start_of_day(std::chrono::system_clock::time_point tp) {
			auto const this_day = chrono::floor<chrono::days>(tp);
			return chrono::year_month_day{ this_day }.operator chrono::sys_days();
		}

		std::chrono::system_clock::time_point get_end_of_day(std::chrono::system_clock::time_point tp) {
			return get_start_of_day(tp) + std::chrono::hours(24);
		}

		std::chrono::system_clock::time_point get_start_of_month(std::chrono::system_clock::time_point tp) {
			auto const this_day = chrono::floor<chrono::days>(tp);
			auto const ymd = chrono::year_month_day{ this_day };
			return chrono::year_month_day{ ymd.year(), ymd.month(), chrono::day{ 1 } }.operator chrono::sys_days();
		}

		std::chrono::system_clock::time_point get_end_of_month(std::chrono::system_clock::time_point tp) {
			auto const this_day = chrono::floor<chrono::days>(tp);
			auto const ymd = chrono::year_month_day{ this_day };
			return chrono::year_month_day_last{ ymd.year(), chrono::month_day_last{ ymd.month()} }.operator chrono::sys_days();
		}

		std::chrono::hours get_length_of_month(std::chrono::system_clock::time_point tp) {
			auto const this_day = chrono::floor<chrono::days>(tp);
			auto const ymd = chrono::year_month_day{ this_day };
			return std::chrono::hours(chrono::year_month_day_last{ ymd.year(), chrono::month_day_last{ ymd.month()} }.day().operator unsigned int() * 24u);
		}

		std::chrono::system_clock::time_point get_end_of_year(std::chrono::system_clock::time_point tp) {
			auto const this_day = chrono::floor<chrono::days>(tp);
			auto const ymd = chrono::year_month_day{ this_day };
			return chrono::year_month_day_last{ ymd.year(), chrono::month_day_last{ chrono::December } }.operator chrono::sys_days();
		}

		std::chrono::system_clock::time_point get_start_of_year(std::chrono::system_clock::time_point tp) {
			auto const this_day = chrono::floor<chrono::days>(tp);
			auto const ymd = chrono::year_month_day{ this_day };
			return chrono::year_month_day{ ymd.year(), chrono::January, chrono::day{ 1 } }.operator chrono::sys_days();
		}

		std::chrono::hours get_length_of_year(std::chrono::system_clock::time_point tp) {
			return std::chrono::duration_cast<std::chrono::hours>(get_end_of_year(tp) - get_start_of_year(tp));
		}

		//std::uint64_t get_iso_week_number(std::chrono::system_clock::time_point tp) {
		//	auto const this_day = chrono::floor<chrono::days>(tp);
		//	auto const this_day_iso = iso_week::year_weeknum_weekday{ this_day };
		//	//auto const this_day_iso = chrono::year_month_weekday{ this_day };

		//	//	2022-W27-Sun
		//	//std::cout << this_day_iso << std::endl;  // 2016-W44-Mon
		//	return this_day_iso.weeknum().operator unsigned int();
		//}

		//std::uint64_t get_day_of_week(std::chrono::system_clock::time_point tp) {
		//	auto const this_day = date::floor<date::days>(tp);
		//	auto const this_day_iso = iso_week::year_weeknum_weekday{ this_day };
		//	return this_day_iso.weekday().operator unsigned int();
		//}
	}
}
