/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_CSV_H
#define CYNG_PARSE_CSV_H


#include <cyng/parse/csv/csv_parser.h>
#include <cyng/parse/csv/line_cast.hpp>

#include <string>

namespace cyng {
	namespace csv {

		template <typename ...Args>
		void parse_typed(std::string const& inp, std::function<void(Args...)> cb, bool header = false, char sep = ',') {
			parser csvp(sep, [cb, &header](line_t&& line) {
				if (!header) {
					header = true;
				}
				else {
					std::apply(cb, line_cast<Args...>(line));
				}
			});

			csvp.read(std::begin(inp), std::end(inp));

		}
	}
}
#endif

