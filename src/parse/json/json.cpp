/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/json.h>
#include <fstream>
#include <iterator>

namespace cyng {
	namespace json {

		object parse(std::string const& inp) {

			object result;
			parser jp([&result](cyng::object&& obj) {
				result = std::move(obj);
				});

			jp.read(std::begin(inp), std::end(inp));

			return object();
		}

		object parse_file(std::string const& file) {

			object result;
			std::ifstream ifs(file);
			if (ifs.is_open())	{

				ifs.unsetf(std::ios::skipws);

				parser jp([&result](cyng::object&& obj) {
					result = std::move(obj);
					});

				auto begin = std::istream_iterator<char>(ifs);
				auto end = std::istream_iterator<char>();
				jp.read(begin, end);

			}
			return result;
		}
	}
}

