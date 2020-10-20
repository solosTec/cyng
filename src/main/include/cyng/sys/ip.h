/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SYS_IP_H
#define CYNG_SYS_IP_H

#include <string>
#include <boost/asio.hpp>
#include <boost/predef.h>

#if BOOST_OS_LINUX
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <string.h>
#endif
	
namespace cyng 
{
	namespace sys 
	{
		/**
		 * Doesn't work obviously behind a NAT.
		 *
		 * @param host an external address/host that is actually reachable
		 * from the client (i.e. "8.8.8.8" or "1.1.1.1")
		 * @return the WAN address
		 */
		boost::asio::ip::address get_WAN_address(std::string host);

		/**
		 * Resolve all addresses of hostname.
		 * 
		 * @return a list of IP addresses from all adapters.
		 */
		std::vector<boost::asio::ip::address> get_adapters();
		
#if BOOST_OS_LINUX		
		boost::asio::ip::address get_ip_address_of_if(std::string ifname);
#endif

	}
}

#endif
