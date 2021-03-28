/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/string.h>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	std::vector<std::string> split(std::string str, std::string delims)
	{
		std::vector<std::string> result;
		boost::algorithm::split(result, str, boost::algorithm::is_any_of(delims), boost::algorithm::token_compress_on);
		return result;
	}

	std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims) {

		std::vector<boost::string_view> result;

		boost::string_view::size_type start = 0;
		auto pos = str.find_first_of(delims, start);
		while (pos != boost::string_view::npos)
		{
			if (pos != start) {
				result.push_back(str.substr(start, pos - start));
			}
			start = pos + 1;
			pos = str.find_first_of(delims, start);
		}

		if (start < str.length()) {
			result.push_back(str.substr(start, str.length() - start));
		}
		return result;
	}


}

