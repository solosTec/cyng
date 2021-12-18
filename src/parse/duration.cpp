/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/duration.h>

#include <algorithm>
#include <tuple>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	namespace {
		std::tuple<std::uint32_t, std::uint32_t, double> split(std::string const& str) {

			std::vector<std::string> parts;
			boost::split(parts, str, boost::is_any_of(":"));
			if (parts.size() == 3) {
				return std::make_tuple(
					static_cast<std::uint32_t>(std::stoul(parts.at(0), nullptr, 10)),
					static_cast<std::uint32_t>(std::stoul(parts.at(1), nullptr, 10)),
					std::stod(parts.at(2))
				);
			}

			return std::make_tuple(0u, 0u, 0.0f);
		}
	}

	std::chrono::nanoseconds to_nanoseconds(std::string const& str) {
		auto const [h, m, s] = split(str);
		return std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(s * 1000ULL * 1000ULL * 1000ULL) + (m * 60ULL * 1000ULL * 1000ULL * 1000ULL) + (h * 60ULL * 60ULL * 1000ULL * 1000ULL * 1000ULL));
	}

	std::chrono::microseconds to_microseconds(std::string const& str) {
		auto const [h, m, s] = split(str);
		return std::chrono::microseconds(static_cast<std::chrono::microseconds::rep>(s * 1000ULL * 1000ULL) + (m * 60ULL * 1000ULL * 1000ULL) + (h * 60ULL * 60ULL * 1000ULL * 1000ULL));
	}

	std::chrono::milliseconds to_milliseconds(std::string const& str) {
		auto const [h, m, s] = split(str);
		return std::chrono::milliseconds(static_cast<std::chrono::milliseconds::rep>(s * 1000ULL) + (m * 60ULL * 1000ULL) + (h * 60ULL * 60ULL * 1000ULL));
	}

	std::chrono::seconds to_seconds(std::string const& str) {
		auto const [h, m, s] = split(str);
		return std::chrono::seconds(static_cast<std::chrono::seconds::rep>(s) + (m * 60ULL) + (h * 60ULL * 60ULL));
	}

	std::chrono::minutes to_minutes(std::string const& str) {
		auto const [h,m,s] = split(str);
		return std::chrono::minutes(static_cast<std::chrono::minutes::rep>(s / 60ULL) + m + (h * 60ULL));
	}

	std::chrono::hours to_hours(std::string const& str) {
		auto const [h, m, s] = split(str);
		return std::chrono::hours(static_cast<std::chrono::hours::rep>(s / 3600) + (m / 60ULL) + h);
	}

}

