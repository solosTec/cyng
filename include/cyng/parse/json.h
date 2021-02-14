/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_JSON_H
#define CYNG_PARSE_JSON_H


#include <cyng/parse/json/json_parser.h>
#include <string>

namespace cyng {
	namespace json {
		object parse(std::string const&);
		object parse_file(std::string const&);
	}
}
#endif

