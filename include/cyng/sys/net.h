/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_NET_H
#define CYNG_SYS_NET_H


#include <string>
#include <vector>
#include <boost/asio/ip/address.hpp>


namespace cyng {
	namespace sys
	{
		/** @brief resolve the specified host name.
		 * 
		 * Requires DNS
		 * 
		 * @return resolved IP address
		 */
		boost::asio::ip::address resolve_address(std::string host);

		/**
		 * @return list of all available nics
		 */
		std::vector<std::string> get_nic_names();
		std::vector<std::string> get_nic_prefix();

		/**
		 * @brief Implementation on windows is incomplete
		 * 
		 *
		 * @return IPv4 and IPv6 address of specified nic
		 */
		boost::asio::ip::address get_address_IPv6(std::string nic);

	}
}
#endif

