/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/parser/ep_parser.h>
#include <cyng/util/split.h>

#include <boost/asio.hpp>

namespace cyng
{
	std::pair<boost::asio::ip::tcp::endpoint, bool > parse_tcp_ep(std::string const& inp)
	{
		auto const vec = split(inp, ":");
		if (vec.size() == 2) {

			try {
				auto const address = boost::asio::ip::make_address(vec.at(0));
				auto const port = std::stoul(vec.at(1));

				return std::make_pair(boost::asio::ip::tcp::endpoint(address, port), true);
			}
			catch (std::invalid_argument) {	}
			catch (std::out_of_range) {	}
			catch (std::exception) {}
		}
		return std::make_pair(boost::asio::ip::tcp::endpoint(), false);
	}

}

 
