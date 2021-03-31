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

		enum ipv6_scope {
			LOOPBACK = 0x0010U,
			LINKLOCAL =	0x0020U,
			SITELOCAL =	0x0040U,
			COMPATv4 = 0x0080U,
			SCOPE_MASK = 0x00f0U
		};

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
		 * The first approach on linux systems with getifaddrs() doesn't work on
		 * oecp devices. The problem is, that adresses of family AF_INET6 are
		 * not listed.
		 * 
		 *
		 * @return IPv4 and IPv6 address of specified nic
		 */
		boost::asio::ip::address get_address_IPv6(std::string nic);

#if defined(BOOST_OS_LINUX_AVAILABLE)
		std::string get_address_IPv6(std::string nic, ipv6_scope);
#endif

	}
}
#endif

