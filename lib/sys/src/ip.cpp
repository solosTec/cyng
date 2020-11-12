/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/ip.h>

#include <iostream>

#include <boost/assert.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng 
{
	namespace sys 
	{
		boost::asio::ip::address get_WAN_address(std::string host)
		{
			try {
				boost::asio::io_service io_service;
				boost::asio::ip::udp::resolver   resolver(io_service);
				boost::asio::ip::udp::socket socket(io_service);
#if (BOOST_ASIO_VERSION < 101400)

				auto res = resolver.resolve(host, "");
				auto ep = res.operator*();
				socket.connect(ep);
#else
				boost::system::error_code ec;
				boost::asio::connect(socket, resolver.resolve(host, ""), ec);
#endif
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
				auto const host = boost::asio::ip::host_name();

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
		
#if BOOST_OS_LINUX		
		boost::asio::ip::address get_ip_address_of_if(std::string ifname)
		{
			struct ifaddrs *ifaddr, *ifa;
			char host[NI_MAXHOST];
			
			//
			//	init ifaddr
			//
			if (getifaddrs(&ifaddr) == -1) return boost::asio::ip::address();
			
			//
			//	loop over all interfaces
			//
			for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
			{
				//
				//	next entry
				//
				if (ifa->ifa_addr == NULL) continue;  
				
				int s = getnameinfo(ifa->ifa_addr
					, sizeof(struct sockaddr_in)
					, host
					, NI_MAXHOST
					, NULL
					, 0
					, NI_NUMERICHOST);
				
				if (boost::algorithm::equals(ifa->ifa_name, ifname) && (ifa->ifa_addr->sa_family==AF_INET))
				{
					if (s == 0) {
#ifdef _DEBUG
						std::cout 
							<< ifa->ifa_name 
							<< " => "
							<< host
							<< std::endl;
#endif
						break;
					}
				}
			}
			
			freeifaddrs(ifaddr);
			return boost::asio::ip::make_address(host);
		}
#else
		//	https://tangentsoft.com/wskfaq/examples/getifaces.html
#endif
		
	}
}




