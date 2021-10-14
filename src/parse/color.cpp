/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/timestamp.h>

#include <algorithm>
#include <tuple>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	namespace {	//	static linkage

		std::tuple<std::uint32_t, std::uint32_t, std::uint32_t> split(std::string const& str, char sep) {

			std::vector<std::string> parts;
			boost::split(parts, str, boost::is_any_of(std::string{ sep }));
			if (parts.size() == 3) {
				return std::make_tuple(
					static_cast<std::uint32_t>(std::stoul(parts.at(0), nullptr, 10)),
					static_cast<std::uint32_t>(std::stoul(parts.at(1), nullptr, 10)),
					static_cast<std::uint32_t>(std::stoul(parts.at(2), nullptr, 10))
				);
			}

			return std::make_tuple(0u, 0u, 0u);
		}
		std::tuple<std::uint32_t, std::uint32_t> split(std::string const& str) {

			std::vector<std::string> parts;
			boost::split(parts, str, boost::is_any_of(":"));
			if (parts.size() == 2) {
				return std::make_tuple(
					static_cast<std::uint32_t>(std::stoul(parts.at(0), nullptr, 10)),
					static_cast<std::uint32_t>(std::stoul(parts.at(1), nullptr, 10))
				);
			}

			return std::make_tuple(0u, 0u);
		}

		std::chrono::system_clock::time_point to_timestamp_date(std::string const& str) {
			//	YYYY-MM-DD
			auto const [year, month, day] = split(str, '-');
			std::tm const tm{
				0,	//	tm_sec - Seconds.	[0-60] (1 leap second)
				0,	//	tm_min - Minutes.	[0-59]
				0,	//	tm_hour - Hours.	[0-23]
				day,	//	tm_mday -  Day.		[1-31]
				month - 1,		//	tm_mon - Month.	[0-11]
				year - 1900,	//	tm_year - Year	- 1900
				//	unused
				0,		//	tm_wday - Day of week  [0-6]
				0,		//	tm_yday - Days in year [0-365]
				-1		//	tm_isdst - DST. [-1/0/1]
			};
			std::time_t const tt = tm_to_tt(tm);
			return std::chrono::system_clock::from_time_t(tt);
		}

		std::chrono::seconds to_timestamp_offset(std::string const& str) {
			//	HH:MM:SS
			auto const [h, m, s] = split(str, ':');
			return std::chrono::seconds(static_cast<std::chrono::seconds::rep>(s) + (m * 60ULL) + (h * 60ULL * 60ULL));
		}

		std::chrono::minutes to_tz_offset(std::string const& str) {
			auto const [h, m] = split(str);
			return std::chrono::minutes(static_cast<std::chrono::minutes::rep>(m) + (h * 60ULL));
		}
	}

	std::chrono::system_clock::time_point to_timestamp(std::string const& str) {
		//	YYYY-MM-DD[THH:MM:SS[Z|[+|-]hh::mm]]
		switch (str.size()) {
		case 10: //	YYYY-MM-DD
			return to_timestamp_date(str);
		case 19: //	YYYY-MM-DDTHH:MM:SS
			if (str.at(10) == 'T') {
				return to_timestamp_date(str) + to_timestamp_offset(str.substr(11));
			}
			break;
		case 20: //	YYYY-MM-DDTHH:MM:SSZ
			if (str.at(10) == 'T' && str.at(19) == 'Z') {
				return to_timestamp_date(str.substr(0, 10)) + to_timestamp_offset(str.substr(11, 8));
			}
		case 25: //	YYYY-MM-DDTHH:MM:SS[+-]hh:mm
			if (str.at(10) == 'T' && str.at(19) == '+') {
				return to_timestamp_date(str.substr(0, 10)) + to_timestamp_offset(str.substr(11, 8)) + to_tz_offset(str.substr(20));
			}
			else if (str.at(10) == 'T' && str.at(19) == '-') {
				return to_timestamp_date(str.substr(0, 10)) + to_timestamp_offset(str.substr(11, 8)) - to_tz_offset(str.substr(20));
			}
			break;
		default:
			break;
		}
		return std::chrono::system_clock::time_point();
	}

	std::time_t tm_to_tt(std::tm const& t)
	{
		std::tm tmp = t;
#if defined(_WIN32) || defined(__CYGWIN__)
		//	The range of the _mkgmtime32 function is from midnight, January 1, 1970, UTC to 23:59:59 January 18, 2038, UTC.
		BOOST_ASSERT_MSG(t.tm_year > 69, "year is out of range");
		return ::_mkgmtime(&tmp);
#else
		//	nonstandard GNU extension, also present on the BSDs
		return ::timegm(&tmp);
#endif
	}
}

