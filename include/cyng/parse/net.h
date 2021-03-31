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


namespace cyng {
	
	/**
	 * Convert a hex string like "2a001e8000640006022518fffef11fd1" into a IPv6 address
	 */
	boost::asio::ip::address to_ipv6(std::string, unsigned long scope_id = 0);

}
#endif

