/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/sys/net.h>

#include <iostream>

#include <boost/predef.h>
#include <boost/assert.hpp>
#include <boost/core/ignore_unused.hpp>


#if defined(BOOST_OS_WINDOWS_AVAILABLE)

#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
//#include <cyng/scm/win_registry.h>

#elif defined(BOOST_OS_LINUX_AVAILABLE)

#include <filesystem>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

#include <boost/algorithm/string.hpp>

#else
#warning unknow OS
#endif

#include <boost/asio.hpp>

namespace cyng 
{
	namespace sys 
	{
		boost::asio::ip::address resolve_address(std::string host) {
			try {
				boost::asio::io_service io_service;
				boost::asio::ip::tcp::resolver   resolver(io_service);
				boost::system::error_code ec;
				auto const results = resolver.resolve(host, "http");
				if (!results.empty())	return results.begin()->endpoint().address();
			}
			catch (std::exception const& ex) {
				boost::ignore_unused(ex);
#ifdef _DEBUG_SYS
				std::cout << ex.what() << std::endl;
#endif				
			}
			return boost::asio::ip::address();

		}

		std::vector<std::string> get_nic_names() {
			std::vector<std::string> nics;

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
			// Allocate information for up to 128 NICs
			IP_ADAPTER_ADDRESSES AdapterInfo[128];

			// Save memory size of buffer
			DWORD dwBufLen = sizeof(AdapterInfo);

			// Arguments for GetAdapterAddresses:
			DWORD dwStatus = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, AdapterInfo, &dwBufLen);
			// [out] buffer to receive data
			// [in] size of receive data buffer


			if (dwStatus == NO_ERROR) {
				// Contains pointer to current adapter info
				PIP_ADAPTER_ADDRESSES pAdapterInfo = AdapterInfo;

				while ((pAdapterInfo != nullptr) && (pAdapterInfo->IfType != IF_TYPE_SOFTWARE_LOOPBACK)) {

#ifdef _DEBUG_SYS
					std::wcout << pAdapterInfo->FriendlyName << '%' << pAdapterInfo->IfIndex << std::endl;
#endif
					int const size = WideCharToMultiByte(CP_UTF8, 0, pAdapterInfo->FriendlyName, -1, NULL, 0, NULL, NULL);

					std::vector<char> utf8(size);
					int const converted = WideCharToMultiByte(CP_UTF8, 0, pAdapterInfo->FriendlyName,
						-1, &utf8[0], utf8.size(), NULL,
						NULL);
					if (converted != 0) {
						nics.push_back(std::string(utf8.begin(), utf8.end()));
					}

					// Progress through linked list
					pAdapterInfo = pAdapterInfo->Next;
				};
			}

#else
			for (auto const& p : std::filesystem::directory_iterator("/sys/class/net")) {
#ifdef _DEBUG_SYS
				std::cout << p.path() << std::endl;
#endif 
				nics.push_back(p.path().filename());

			}
#endif
			return nics;
		}


		std::vector<std::string> get_nic_prefix() {


#if defined(BOOST_OS_WINDOWS_AVAILABLE)

			std::vector<std::string> prefix;

			// Allocate information for up to 128 NICs
			IP_ADAPTER_ADDRESSES AdapterInfo[128];

			// Save memory size of buffer
			DWORD dwBufLen = sizeof(AdapterInfo);

			// Arguments for GetAdapterAddresses:
			DWORD dwStatus = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, AdapterInfo, &dwBufLen);
			// [out] buffer to receive data
			// [in] size of receive data buffer


			if (dwStatus == NO_ERROR) {
				// Contains pointer to current adapter info
				PIP_ADAPTER_ADDRESSES pAdapterInfo = AdapterInfo;

				while ((pAdapterInfo != nullptr) && (pAdapterInfo->IfType != IF_TYPE_SOFTWARE_LOOPBACK)) {

#ifdef _DEBUG_SYS
					std::wcout << pAdapterInfo->FriendlyName << '%' << pAdapterInfo->IfIndex << std::endl;
					BOOST_ASSERT(pAdapterInfo->IfIndex == pAdapterInfo->Ipv6IfIndex);
#endif
					prefix.push_back(std::to_string(pAdapterInfo->IfIndex));

					// Progress through linked list
					pAdapterInfo = pAdapterInfo->Next;
				};
			}

			return prefix;

#else
			return get_nic_names();
#endif


		}

		boost::asio::ip::address get_address_IPv6(std::string nic) {

#if defined(BOOST_OS_WINDOWS_AVAILABLE)

			std::vector<std::string> prefix;

			// Allocate information for up to 128 NICs
			IP_ADAPTER_ADDRESSES AdapterInfo[128];

			// Save memory size of buffer
			DWORD dwBufLen = sizeof(AdapterInfo);

			// Arguments for GetAdapterAddresses:
			DWORD dwStatus = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, AdapterInfo, &dwBufLen);
			// [out] buffer to receive data
			// [in] size of receive data buffer


			if (dwStatus == NO_ERROR) {
				// Contains pointer to current adapter info
				PIP_ADAPTER_ADDRESSES pAdapterInfo = AdapterInfo;

				while ((pAdapterInfo != nullptr) && (pAdapterInfo->IfType != IF_TYPE_SOFTWARE_LOOPBACK)) {

#ifdef _DEBUG_SYS
					std::wcout << pAdapterInfo->FriendlyName << '%' << pAdapterInfo->IfIndex << std::endl;
					BOOST_ASSERT(pAdapterInfo->IfIndex == pAdapterInfo->Ipv6IfIndex);
					if (pAdapterInfo->FirstPrefix) {
						auto pPrefix = pAdapterInfo->FirstPrefix;
						int i = 0;
						for (; pPrefix != NULL; i++) {
							//boost::asio::ip::address_v4 addr(pPrefix->Address.lpSockaddr);
							
							pPrefix = pPrefix->Next;
						}
						printf("\tNumber of IP Adapter Prefix entries: %d\n", i);
					}
#endif
					//prefix.push_back(std::to_string(pAdapterInfo->IfIndex));

					// Progress through linked list
					pAdapterInfo = pAdapterInfo->Next;
				};
			}
			return boost::asio::ip::address();

#else
			struct ifaddrs* ifaddr, * ifa;
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
				auto const family = ifa->ifa_addr->sa_family;

				if (family == AF_INET6) 	{
					int s = getnameinfo(ifa->ifa_addr
						, sizeof(struct sockaddr_in6)
						, host
						, NI_MAXHOST
						, NULL
						, 0
						, NI_NUMERICHOST);

					if (boost::algorithm::equals(ifa->ifa_name, nic) && (ifa->ifa_addr->sa_family == AF_INET6))
					{
						if (s == 0) {
#ifdef _DEBUG_SYS
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
			}

			freeifaddrs(ifaddr);
			return boost::asio::ip::make_address(host);

#endif

		}

	}
}




