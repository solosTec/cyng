/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/ip.h>
#include <boost/assert.hpp>
#include <boost/core/ignore_unused.hpp>
#include <iostream>

namespace cyng 
{
	namespace sys 
	{
		boost::asio::ip::address get_WAN_address(std::string host)
		{
			try {
				boost::asio::io_service io_service;
				boost::asio::ip::udp::resolver   resolver(io_service);
				auto res = resolver.resolve(host, "");
				auto ep = res.operator*();
				boost::asio::ip::udp::socket socket(io_service);
				socket.connect(ep);
				return socket.local_endpoint().address();
			}
			catch (std::exception const& ex) {
				boost::ignore_unused(ex);
			}
			return boost::asio::ip::address();
		}

		std::vector<boost::asio::ip::address> get_adapters()
		{
			std::vector<boost::asio::ip::address> addrs;

			try
			{
				//
				//	get hostname
				//
				std::string const host = boost::asio::ip::host_name();

				//
				//	resolve all addresses of this hostname
				//
				boost::asio::io_service io_service;
				boost::asio::ip::tcp::resolver resolver(io_service);
				auto res = resolver.resolve(host, "");
				for (auto pos = res.begin(); pos != res.end(); ++pos) {
					boost::asio::ip::tcp::endpoint ep = *pos;
					addrs.push_back(ep.address());
				}
			}
			catch (std::exception const& ex) {
				boost::ignore_unused(ex);
			}

			return addrs;
		}
	}
}




