/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/util/split.h>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>	//	is_any_of

namespace cyng
{
#if defined(__CPP_SUPPORT_N3921)    
	std::vector<std::string_view> split(std::string_view str, std::string_view delims)
	{
		std::vector<std::string_view> ret;
		
		std::string_view::size_type start = 0;
		auto pos = str.find_first_of(delims, start);
		while (pos != std::string_view::npos)
		{
			if (pos != start)
			{
				ret.push_back(str.substr(start, pos - start));
			}
			start = pos + 1;
			pos = str.find_first_of(delims, start);
		}
		if (start < str.length())
		{
			ret.push_back(str.substr(start, str.length() - start));
		}
		return ret;
	}
#endif

	std::vector<boost::string_view> split(boost::string_view str, boost::string_view delims)
	{
		std::vector<boost::string_view> ret;
		
		boost::string_view::size_type start = 0;
		auto pos = str.find_first_of(delims, start);
		while (pos != boost::string_view::npos)
		{
			if (pos != start)
			{
				ret.push_back(str.substr(start, pos - start));
			}
			start = pos + 1;
			pos = str.find_first_of(delims, start);
		}
		if (start < str.length())
		{
			ret.push_back(str.substr(start, str.length() - start));
		}
		return ret;
	}
	
	std::vector<std::string> split(std::string str, std::string delims)
	{
		std::vector<std::string> result;
		boost::algorithm::split(result, str, boost::algorithm::is_any_of(delims), boost::algorithm::token_compress_on);
		return result;
	}
}

