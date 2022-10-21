/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_NET_H
#define CYNG_PARSE_NET_H


#include <string>
#include <boost/asio/ip/address.hpp>
#include <boost/predef.h>

namespace cyng {
	
	/**
	 * Convert a hex string like "2a001e8000640006022518fffef11fd1" into a IPv6 address
	 */
	boost::asio::ip::address to_ipv6(std::string hex_str);

#if defined(BOOST_OS_LINUX_AVAILABLE)
	/**
	 * Convert a hex string like "2a001e8000640006022518fffef11fd1" into a IPv6 address
	 * that is bound to an interface.
	 * format: ipv6%device 
	 * This is NOT standard and only supported on linux/UNIX systems
	 */
	[[deprecated("use an address scoped by index")]]
	boost::asio::ip::address to_ipv6(std::string hex_str, std::string device);
#endif

}
#endif

