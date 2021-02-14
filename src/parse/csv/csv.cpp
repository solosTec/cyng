/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/csv.h>
#include <cyng/obj/factory.hpp>
#include <cyng/obj/container_cast.hpp>
#include <fstream>

namespace cyng {

	namespace csv {
		vector_t parse(std::string const& inp, char sep) {

			vector_t result;
			parser csvp(sep, [&result](vector_t&& vec) {
				result.push_back(make_object(std::move(vec)));
				});

			csvp.read(std::begin(inp), std::end(inp));

			return (result.size() == 1)
				? container_cast<vector_t>(result.at(0))
				: result
				;
		}

		vector_t parse_file(std::string const& file, char sep) {

			vector_t result;
			std::ifstream ifs(file);
			if (ifs.is_open()) {

				ifs.unsetf(std::ios::skipws);

				parser csvp(sep, [&result](vector_t&& vec) {
					result.push_back(make_object(std::move(vec)));
					});

				auto begin = std::istream_iterator<char>(ifs);
				auto end = std::istream_iterator<char>();
				csvp.read(begin, end);

			}
			return (result.size() == 1)
				? container_cast<vector_t>(result.at(0))
				: result
				;
		}
	}

}

