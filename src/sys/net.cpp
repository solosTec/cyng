/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/sys/net.h>
#include <cyng/parse/net.h>

#include <iostream>
#include <iomanip>

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
#include <fstream>

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

#if defined(BOOST_OS_LINUX_AVAILABLE)
		void read_ipv6_info(std::function<bool(std::string, std::string, std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t)> cb) {
			std::ifstream ifs("/proc/net/if_inet6");
			ifs >> std::setbase(16);
			std::string address, name;
			std::uint64_t index, len, scope, flag;
			while (ifs) {
				ifs >> address >> index >> len >> scope >> flag >> name;
				if (!cb(address, name, index, len, scope, flag))	break;
			}
		}

#endif

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
			boost::asio::ip::address result;
			read_ipv6_info([&result, nic](std::string address, std::string name, std::uint64_t index, std::uint64_t len, std::uint64_t scope, std::uint64_t flag) -> bool {
				//std::cout << address << " - " << name << std::endl;
				if (boost::algorithm::equals(name, nic))	{
				//	std::cout << to_ipv6(address, scope) << std::endl;
					result = to_ipv6(address, scope);
					return false;
				}
				return true;
				});
			return result;

#endif

		}

#if defined(BOOST_OS_LINUX_AVAILABLE)
		std::string get_address_IPv6(std::string nic, ipv6_scope sc) {

			std::string result("0.0.0.0");
			read_ipv6_info([&result, nic, sc](std::string address, std::string name, std::uint64_t index, std::uint64_t len, std::uint64_t scope, std::uint64_t flag) -> bool {
				//std::cout << address << " - " << name << " - " << std::hex << scope << std::endl;
				if (boost::algorithm::equals(name, nic) && sc == scope) {
					// std::cout << to_ipv6(address, scope) << std::endl;
					result = (scope == LINKLOCAL)
						? to_ipv6(address, scope).to_string() + "%" + name 
						: to_ipv6(address, scope).to_string()
						;
					return false;
				}
				return true;
				});
			return result;

		}
#endif

	}
}




