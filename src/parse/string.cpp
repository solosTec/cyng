/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/string.h>

#include <iomanip>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/asio.hpp>

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


	boost::uuids::uuid to_uuid(std::string const& str) {
		
		try {
			return boost::uuids::string_generator()(str);
		}
		catch (std::exception const&) {}
		return boost::uuids::nil_uuid();
	}

	//	2021-03-31T19:36:45+0100	- ISO-8601
	std::chrono::system_clock::time_point to_tp_iso8601(std::string const& str) {

		std::tm tm = {};
		std::stringstream ss(str);
		ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S%z");
		return std::chrono::system_clock::from_time_t(std::mktime(&tm));
	}

	//	"%Y-%m-%d %H:%M:%S" - SQLite datetime()
	std::chrono::system_clock::time_point to_tp_datetime(std::string const& str) {

		std::tm tm = {};
		std::stringstream ss(str);
		ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
		return std::chrono::system_clock::from_time_t(std::mktime(&tm));

	}

	boost::asio::ip::address to_ip_address(std::string const& str) {

		boost::system::error_code ec;
		return boost::asio::ip::make_address(str, ec);

	}
}

