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

#include <boost/utility/string_view.hpp>

namespace cyng {
	std::vector<std::string> split(std::string str, std::string delims);
	std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims);

}
#endif

