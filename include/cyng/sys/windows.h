/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_WINDOWS_HPP
#define CYNG_SYS_WINDOWS_HPP

/** @file windows.hpp
 * Some Windows specific tools to get system information
 */

#include <string>
#include <cstddef>
#include <boost/predef.h>
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
#include <boost/asio.hpp>	//	 Iphlpapi.h h
#include <iphlpapi.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#pragma comment(lib, "IPHLPAPI.lib")


namespace cyng {
	namespace sys
	{

		std::string convert_to_utf8(PWCHAR name);

		/**
		 * Generic method to iterate over all IPv6 addresses of the system.
		 * This function modelled after the example at https://docs.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_addresses_lh
		 */
		using ip_address_cb = std::function<bool(IP_ADAPTER_ADDRESSES const&, std::string)>;

		[[nodiscard]]
		PIP_ADAPTER_ADDRESSES get_adapter_adresses(ULONG family);
	}
}
#endif
#endif

