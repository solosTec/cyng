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
	//	select namespace - doesn't work 
	//
// #if defined(CYNG_USE_DATE_LIBRARY)
// 	namespace chrono = date;
// #else
// 	namespace chrono = std::chrono;
// #endif

	namespace sys
	{
		std::chrono::system_clock::time_point get_start_of_day(std::chrono::system_clock::time_point tp) {
#if defined(CYNG_USE_DATE_LIBRARY)
			auto const this_day = date::floor<date::days>(tp);
			return date::year_month_day{ this_day }.operator date::sys_days();
#else			
			auto const this_day = std::chrono::floor<std::chrono::days>(tp);
			return std::chrono::year_month_day{ this_day }.operator std::chrono::sys_days();
#endif
		}

		std::chrono::system_clock::time_point get_end_of_day(std::chrono::system_clock::time_point tp) {
			return get_start_of_day(tp) + std::chrono::hours(24);
		}

		std::chrono::system_clock::time_point get_start_of_month(std::chrono::system_clock::time_point tp) {
#if defined(CYNG_USE_DATE_LIBRARY)
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return date::year_month_day{ ymd.year(), ymd.month(), date::day{ 1 } }.operator date::sys_days();
#else
			auto const this_day = std::chrono::floor<std::chrono::days>(tp);
			auto const ymd = std::chrono::year_month_day{ this_day };
			return std::chrono::year_month_day{ ymd.year(), ymd.month(), std::chrono::day{ 1 } }.operator std::chrono::sys_days();
#endif
		}

		std::chrono::system_clock::time_point get_end_of_month(std::chrono::system_clock::time_point tp) {
#if defined(CYNG_USE_DATE_LIBRARY)
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return date::year_month_day_last{ ymd.year(), date::month_day_last{ ymd.month()} }.operator date::sys_days();
#else
			auto const this_day = std::chrono::floor<std::chrono::days>(tp);
			auto const ymd = std::chrono::year_month_day{ this_day };
			return std::chrono::year_month_day_last{ ymd.year(), std::chrono::month_day_last{ ymd.month()} }.operator std::chrono::sys_days();
#endif
		}

		std::chrono::hours get_length_of_month(std::chrono::system_clock::time_point tp) {
#if defined(CYNG_USE_DATE_LIBRARY)
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return std::chrono::hours(date::year_month_day_last{ ymd.year(), date::month_day_last{ ymd.month()} }.day().operator unsigned int() * 24u);
#else
			auto const this_day = std::chrono::floor<std::chrono::days>(tp);
			auto const ymd = std::chrono::year_month_day{ this_day };
			return std::chrono::hours(std::chrono::year_month_day_last{ ymd.year(), std::chrono::month_day_last{ ymd.month()} }.day().operator unsigned int() * 24u);
#endif
		}

		std::chrono::system_clock::time_point get_end_of_year(std::chrono::system_clock::time_point tp) {
#if defined(CYNG_USE_DATE_LIBRARY)
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return  date::year_month_day_last{ ymd.year(), date::month_day_last{ date::December } }.operator date::sys_days();
#else
			auto const this_day = std::chrono::floor<std::chrono::days>(tp);
			auto const ymd = std::chrono::year_month_day{ this_day };
			return std::chrono::year_month_day_last{ ymd.year(), std::chrono::month_day_last{ std::chrono::December } }.operator std::chrono::sys_days();
#endif
		}

		std::chrono::system_clock::time_point get_start_of_year(std::chrono::system_clock::time_point tp) {
#if defined(CYNG_USE_DATE_LIBRARY)
			auto const this_day = date::floor<date::days>(tp);
			auto const ymd = date::year_month_day{ this_day };
			return date::year_month_day{ ymd.year(), date::January, date::day{ 1 } }.operator date::sys_days();
#else
			auto const this_day = std::chrono::floor<std::chrono::days>(tp);
			auto const ymd = std::chrono::year_month_day{ this_day };
			return std::chrono::year_month_day{ ymd.year(), std::chrono::January, std::chrono::day{ 1 } }.operator std::chrono::sys_days();
#endif
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
