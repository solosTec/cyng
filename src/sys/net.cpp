/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/sys/net.h>
#include <cyng/parse/net.h>
#include <cyng/obj/algorithm/swap_bytes.h>

#include <iostream>
#include <iomanip>

#include <boost/predef.h>
#include <boost/assert.hpp>
#include <boost/core/ignore_unused.hpp>


#if defined(BOOST_OS_WINDOWS_AVAILABLE)

#include <cyng/sys/windows.h>

#elif defined(BOOST_OS_LINUX_AVAILABLE)

#include <filesystem>
#include <fstream>
#include <cyng/sys/linux.hpp>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <net/if.h>
#include <ifaddrs.h>

#else
#warning unknow OS
#endif

#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>

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

#if defined(BOOST_OS_LINUX_AVAILABLE)
		namespace {	//	static linkage
			void read_ipv6_info(std::function<bool(std::string, std::string, std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t)> cb) {
				std::ifstream ifs("/proc/net/if_inet6");
				std::string address, name, line;
				std::uint64_t index, len, scope, flag;
				while (std::getline(ifs, line)) {
					//ifs >> std::setbase(16);
					std::istringstream(line) >> address >> std::hex >> index >> len >> scope >> flag >> name;
					if (!cb(address, name, index, len, scope, flag))	break;
				}
			}

			void get_nic_names_linux(std::vector<std::string>& nics) {
				for (auto const& p : std::filesystem::directory_iterator("/sys/class/net")) {
					//	skip loopback
					auto const device = p.path().filename().string();
					if (!boost::algorithm::equals(device, "lo")) {
						nics.push_back(p.path().filename());
					}
				}
			}
		}
#endif

#if defined(BOOST_OS_WINDOWS_AVAILABLE)

		namespace {	//	static linkage

			void read_ipv6_info(ip_address_cb cb) {

				//
				//	read IPv6 adapter info
				//
				auto p_begin = get_adapter_adresses(AF_INET6);

				//
				//	exit loop if callback function returns false
				//
				auto pos = p_begin;
				while ((pos != nullptr) && cb(*pos, convert_to_utf8(pos->FriendlyName))) {
					//
					//	next address
					//
					pos = pos->Next;
				}

				//
				//	free memory
				//
				free(p_begin);

			}

			/**
			 * Generic method to iterate over all IPv4 addresses of the system.
			 * This function modelled after the example at https://docs.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_addresses_lh
			 */
			void read_ipv4_info(ip_address_cb cb) {

				//
				//	read IPv4 adapter info
				//
				auto p_begin = get_adapter_adresses(AF_INET);

				//
				//	exit loop if callback function returns false
				//
				auto pos = p_begin;
				while ((pos != nullptr) && cb(*pos, convert_to_utf8(pos->FriendlyName))) {
					//
					//	next address
					//
					pos = pos->Next;
				}

				//
				//	free memory
				//
				free(p_begin);
			}


			void read_ip_info(ip_address_cb cb) {
				//
				//	read adapter info
				//
				read_ipv4_info(cb);
				read_ipv6_info(cb);

			}

			void get_nic_names_windows(std::vector<std::string>& nics) {
				read_ip_info([&](IP_ADAPTER_ADDRESSES const& address, std::string name)->bool {
					if (address.IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
						nics.push_back(name);
					}
					return true;
					}
				);

			}
		}
#endif

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
		std::vector<ipv_cfg> get_ipv4_configuration_windows() {
			std::vector<ipv_cfg> r;
			read_ipv4_info([&r](IP_ADAPTER_ADDRESSES const& address, std::string name)->bool {

				//
				//	skip loopback 
				//
				if (address.IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
					if (address.FirstUnicastAddress != nullptr) {
						auto pUniscastAddress = address.FirstUnicastAddress;
						for (; pUniscastAddress != NULL; ) {
							if (pUniscastAddress->Address.lpSockaddr->sa_family == AF_INET) {

								sockaddr_in* p = (sockaddr_in*)pUniscastAddress->Address.lpSockaddr;
								boost::asio::ip::address_v4::bytes_type bytes = { 0 };
								bytes.at(0) = p->sin_addr.S_un.S_un_b.s_b1;
								bytes.at(1) = p->sin_addr.S_un.S_un_b.s_b2;
								bytes.at(2) = p->sin_addr.S_un.S_un_b.s_b3;
								bytes.at(3) = p->sin_addr.S_un.S_un_b.s_b4;
								r.emplace_back(boost::asio::ip::make_address_v4(bytes), address.IfIndex, name);
								//r.emplace_back(boost::asio::ip::make_address_v4(p->sin_addr.S_un.S_addr), address.IfIndex, name);
							}

							pUniscastAddress = pUniscastAddress->Next;
						}
					}
				}
				return true;	//	read more
				});

			return r;

		}

		std::vector<ipv_cfg> get_ipv6_configuration_windows() {
			std::vector<ipv_cfg> r;
			read_ipv6_info([&r](IP_ADAPTER_ADDRESSES const& address, std::string name)->bool {

				//
				//	skip loopback 
				//
				if (address.IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
					if (address.FirstUnicastAddress != nullptr) {
						auto pUnicastAddress = address.FirstUnicastAddress;
						for (; pUnicastAddress != NULL; ) {
							if (pUnicastAddress->Address.lpSockaddr->sa_family == AF_INET6) {

								sockaddr_in6* p6 = (sockaddr_in6*)pUnicastAddress->Address.lpSockaddr;
								boost::asio::ip::address_v6::bytes_type bytes = { 0 };
								for (std::size_t pos = 0; pos < bytes.size(); ++pos) {
									bytes.at(pos) = static_cast<unsigned char>(p6->sin6_addr.u.Byte[pos]);
								}

								r.emplace_back(boost::asio::ip::make_address_v6(bytes), address.Ipv6IfIndex, name);
							}

							pUnicastAddress = pUnicastAddress->Next;
						}
					}
				}
				return true;
				});

			return r;

		}
#endif

#if defined(BOOST_OS_LINUX_AVAILABLE)
		std::vector<ipv_cfg> get_ipv4_configuration_posix() {
			std::vector<ipv_cfg> r;
			//	use getifaddrs()
			struct ifaddrs *interfaces = NULL;
			auto const success = getifaddrs(&interfaces);
			if (success == 0) {

				auto temp_addr = interfaces;
				while(temp_addr != NULL) {
					if(temp_addr->ifa_addr->sa_family == AF_INET) { 
						bool const is_loopback = (0 != (temp_addr->ifa_flags & IFF_LOOPBACK)); 
						if (!is_loopback) {
							boost::asio::ip::address_v4::bytes_type bytes = { 0 };
							sockaddr_in* p = (sockaddr_in*) temp_addr->ifa_addr;		
							auto const addr = boost::asio::ip::make_address_v4(swap_bytes(p->sin_addr.s_addr));
							r.emplace_back(addr, if_nametoindex(temp_addr->ifa_name), temp_addr->ifa_name);
						}
					}
					//
					//	next entry
					//
					temp_addr = temp_addr->ifa_next;
				}
			}
			freeifaddrs(interfaces); 
			return r;
		}
		std::vector<ipv_cfg> get_ipv6_configuration_posix() {
			std::vector<ipv_cfg> r;
			read_ipv6_info([&r](std::string address, std::string name, std::uint64_t index, std::uint64_t len, std::uint64_t scope, std::uint64_t flag) -> bool {
				// std::cout << "\t***" << address << " - " << name << std::endl;
				if (scope != LOOPBACK) {
					r.emplace_back(to_ipv6(address), index, name);
				}
				return true;
				});
			return r;
		}
#endif

		std::vector<ipv_cfg> get_ipv4_configuration() {
			//
			//	https://en.wikipedia.org/wiki/Memoization
			//
			static std::vector<ipv_cfg> r;
			if (r.empty()) {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
				r = get_ipv4_configuration_windows();
#elif defined(BOOST_OS_LINUX_AVAILABLE)
				r = get_ipv4_configuration_posix();
#else
				static_assert(false, "platform not supported");
#endif
			}
			return r;

		}


		std::vector<ipv_cfg> get_ipv6_configuration() {
			//
			//	https://en.wikipedia.org/wiki/Memoization
			//
			static std::vector<ipv_cfg> r;
			if (r.empty()) {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
				r = get_ipv6_configuration_windows();
#elif defined(BOOST_OS_LINUX_AVAILABLE)
				r = get_ipv6_configuration_posix();
#else
				static_assert(false, "platform not supported");
#endif
			}
			return r;
		}

		std::vector<std::string> get_nic_names() {
			static std::vector<std::string> nics;
			if (nics.empty()) {

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
				get_nic_names_windows(nics);
#else
				get_nic_names_linux(nics);
#endif

			}
			return nics;
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
						int iv4 = 0, iv6 = 0;
						for (; pPrefix != NULL; ) {
							if (pPrefix->Address.lpSockaddr->sa_family == AF_INET) {
								//boost::asio::ip::address_v4 addr(pPrefix->Address.lpSockaddr);
								++iv4;
							}
							else if (pPrefix->Address.lpSockaddr->sa_family == AF_INET6){

								//boost::asio::ip::address_v6 addr(&pPrefix->Address.lpSockaddr->sa_data);
								++iv6;
							}
							
							pPrefix = pPrefix->Next;
						}
						printf("\tNumber of IP Adapter Prefix entries: %d x IPv4, %d x IPv6\n", iv4, iv6);
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
					//	std::cout << to_ipv6(address, name) << std::endl;
					result = to_ipv6(address);
					return false;
				}
				return true;
				});
			return result;

#endif

		}

#if defined(BOOST_OS_LINUX_AVAILABLE)
		boost::asio::ip::address get_address_IPv6(std::string nic, ipv6_scope sc) {

			boost::asio::ip::address result;
			read_ipv6_info([&result, nic, sc](std::string address, std::string name, std::uint64_t index, std::uint64_t len, std::uint64_t scope, std::uint64_t flag) -> bool {
				//std::cout << address << " - " << name << " - " << scope << std::endl;
				if (boost::algorithm::equals(name, nic) && sc == scope) {
					//std::cout << to_ipv6(address) << std::endl;
					result = to_ipv6(address);
					return false;
				}
				return true;
				});
			return result;

		}
#endif

		ipv_cfg::ipv_cfg(boost::asio::ip::address address, std::uint32_t index, std::string device)
			: address_(address)
			, index_(index)
			, device_(device)
		{}

		std::ostream& operator<<(std::ostream& os, const ipv_cfg& data) {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)
			os << '[' << data.device_ << ']' << ' ' << data.address_ << '%' << data.index_;
#else
			os << '[' << data.index_ << ']' << ' ' << data.address_ << '%' << data.device_;
#endif
			return os;
		}

		filter_by_name::filter_by_name(std::string device)
			: device_(device)
		{}
		bool filter_by_name::operator()(ipv_cfg const& cfg) const {
			return boost::algorithm::equals(cfg.device_, device_);
		}

		filter_by_index::filter_by_index(std::uint32_t index)
			: index_(index)
		{}
		bool filter_by_index::operator()(ipv_cfg const& cfg) const {
			return cfg.index_ == index_;
		}

		filter_by_address::filter_by_address(boost::asio::ip::address address)
			: address_(address)
		{}
		bool filter_by_address::operator()(ipv_cfg const& cfg) const {
			return cfg.address_ == address_;
		}

		std::vector<ipv_cfg> merge(std::vector<ipv_cfg> const& a, std::vector<ipv_cfg> const& b) {

			std::vector<ipv_cfg> r;
			for (auto const& cfg : a) {
				r.push_back(cfg);
				//
				//	add all entries of b with the same index
				//
				auto const cfg_filtered = cyng::sys::filter(b, cyng::sys::filter_by_index(cfg.index_));
				r.insert(r.end(), cfg_filtered.begin(), cfg_filtered.end());
			}
			return r;
		}

	}
}




