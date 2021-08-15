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
#include <cstdint>
#include <ostream>
#include <algorithm>

#include <boost/asio/ip/address.hpp>
#include <boost/predef.h>

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

		/**
		 * contains IPv4/IPv6 configuration data
		 */
		struct ipv_cfg {
			boost::asio::ip::address address_;	//!<	IPv4/IPv6 address
			std::uint32_t index_;
			std::string device_;	//!<	adapter name
			ipv_cfg(boost::asio::ip::address address, std::uint32_t index, std::string device);
		};

		/**
		 * Note that the print format varies depending on the platform.
		 */
		std::ostream& operator<<(std::ostream& os, const ipv_cfg& data);

		template <typename F>
		std::vector<ipv_cfg> filter(std::vector<ipv_cfg> const& inp, F f) {
			std::vector<ipv_cfg> r;
			std::copy_if(std::begin(inp), std::end(inp), std::back_inserter(r), [f](ipv_cfg const& cfg) {
				return f(cfg);
				});
			return r;
		}

		/**
		 * some predfined filters
		 */
		struct filter_by_name {
			filter_by_name(std::string device);
			std::string const device_;	//!<	adapter name
			bool operator()(ipv_cfg const& cfg) const;

		};
		struct filter_by_index {
			filter_by_index(std::uint32_t);
			std::uint32_t const index_;
			bool operator()(ipv_cfg const& cfg) const;
		};
		struct filter_by_address {
			filter_by_address(boost::asio::ip::address);
			boost::asio::ip::address const address_;	//!<	IPv4/IPv6 address
			bool operator()(ipv_cfg const& cfg) const;
		};

		/**
		 * The order of the first vector is retained.
		 */
		std::vector<ipv_cfg> merge(std::vector<ipv_cfg> const&, std::vector<ipv_cfg> const&);


		/** @brief resolve the specified host name.
		 * 
		 * Requires DNS
		 * 
		 * @return resolved IP address
		 */
		boost::asio::ip::address resolve_address(std::string host);

		/**
		 * This is a platform independed way to get all IPv4 addresses
		 * on this system.
		 * In linux systems this data are gathered by using the getifaddrs() function.
		 * On windows this happens by calling the GetAdaptersAddresses() function.
		 */
		std::vector<ipv_cfg> get_ipv4_configuration();

		/**
		 * This is a platform independed way to get all IPv6 addresses 
		 * on this system.
		 * In linux systems this data are gathered by reding the "/proc/net/if_inet6" file.
		 * On windows this happens by calling the GetAdaptersAddresses() function.
		 */
		std::vector<ipv_cfg> get_ipv6_configuration();

		/**
		 * @return list of all available nics
		 */
		std::vector<std::string> get_nic_names();

		/**
		 * on linux the same as get_nic_names()
		 */
		[[deprecated("Use get_nic_names() instead.")]]
		std::vector<std::string> get_nic_prefix();

		/**
		 * @brief Implementation on windows is incomplete. Use get_ipv6_configuration() instead.
		 * 
		 * The first approach on linux systems with getifaddrs() doesn't work on
		 * oecp devices. The problem is, that adresses of family AF_INET6 are
		 * not listed.
		 * 
		 *
		 * @return IPv4 and IPv6 address of specified nic
		 */
		[[deprecated("Use get_ipv6_configuration() instead.")]]
		boost::asio::ip::address get_address_IPv6(std::string nic);

#if defined(BOOST_OS_LINUX_AVAILABLE)
		[[deprecated("Use get_ipv6_configuration() instead.")]]
		boost::asio::ip::address get_address_IPv6(std::string nic, ipv6_scope);
		//void read_ipv6_info(std::function<bool(std::string, std::string, std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t)>);
#endif

	}
}
#endif

