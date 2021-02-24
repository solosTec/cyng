/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_STRING_H
#define CYNG_PARSE_STRING_H


#include <string>
#include <vector>

namespace cyng {
	std::vector<std::string> split(std::string str, std::string delims);
}
#endif

