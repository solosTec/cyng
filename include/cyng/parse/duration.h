/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_DURATION_H
#define CYNG_PARSE_DURATION_H


#include <chrono>
#include <string>

namespace cyng {

	/**
	 * expected format is "00:00:1.000000"
	 */
	std::chrono::microseconds to_microseconds(std::string const& str);

	/**
	 * expected format is "00:00:1.000000"
	 */
	std::chrono::milliseconds to_milliseconds(std::string const& str);

	/**
	 * expected format is "00:00:1.000000"
	 */
	std::chrono::seconds to_seconds(std::string const& str);

	/**
	 * expected format is "00:00:1.000000"
	 */
	std::chrono::minutes to_minutes(std::string const& str);

	/**
	 * expected format is "00:00:1.000000"
	 */
	std::chrono::hours to_hours(std::string const& str);
}
#endif

