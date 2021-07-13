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
#include <functional>

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

		/**
		 * Takes the first line as column names and produce with the following lines maps
		 */
		class parser_named
		{
		public:
			using cb_f = std::function<void(std::map<std::string, std::string> const&, std::size_t)>;
		public:
			parser_named(cb_f cb, char sep = ',');

			template < typename I >
			auto read(I start, I end) -> typename std::iterator_traits<I>::difference_type {
				return parser_.read(start, end);
			}
		private:
			cb_f cb_;
			line_t header_;
			std::size_t counter_;
			parser parser_;	//!< csv parser
		};

	}
}
#endif

