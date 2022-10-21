/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_TIMESTAMP_H
#define CYNG_PARSE_TIMESTAMP_H


#include <chrono>
#include <string>

namespace cyng {

	/**
	 * expected format is "YYYY-MM-DD[THH:MM:SS[Z|[+|-]hh::mm]]"
	 */
	std::chrono::system_clock::time_point to_timestamp(std::string const& str);

	std::time_t tm_to_tt(std::tm const& t);

}
#endif

