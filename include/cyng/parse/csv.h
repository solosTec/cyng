/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_CSV_H
#define CYNG_PARSE_CSV_H


#include <cyng/parse/csv/csv_parser.h>
#include <string>

namespace cyng {
	namespace csv {
		vector_t parse(std::string const&, char sep = ',');
		vector_t parse_file(std::string const&, char sep = ',');
	}
}
#endif

