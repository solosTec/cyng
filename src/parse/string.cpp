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

}

