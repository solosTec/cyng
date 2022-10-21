/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/sys/dns.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include <cyng/sys/windows.h>
#include <stdio.h>
#include <windows.h>

#define DNS_MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define DNS_FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#elif BOOST_OS_LINUX

#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <netdb.h>
#include <arpa/inet.h>

//
//	alpine linux <resolv.h>(musl) doesn't implement functions
//	like res_ninit(&rs)
//
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <features.h>
#ifndef __USE_GNU
#define __MUSL__ 
#endif
#undef _GNU_SOURCE // don't contaminate other includes unnecessarily

#else
#warning unknow OS
#endif

namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX
		std::vector<boost::asio::ip::address>	get_dns_servers()
		{
            std::vector<boost::asio::ip::address> dns;
			//
			//	alpine linux <resolv.h> (musl) doesn't implement these functions
			//
#if !defined(__MUSL__)
			struct __res_state rs;
 			::res_ninit(&rs);
			for (int i = 0; i < rs.nscount; i++) {
				sa_family_t family = rs.nsaddr_list[i].sin_family;
				if (family == AF_INET) { // IPV4 address
					char str[INET_ADDRSTRLEN]; // String representation of address
					inet_ntop(AF_INET, &(rs.nsaddr_list[i].sin_addr.s_addr), str, INET_ADDRSTRLEN);
//                     printf("%s\n", str);
                    dns.push_back(boost::asio::ip::address::from_string(str));
				}
				else if (family == AF_INET6) { // IPV6 address
					char str[INET6_ADDRSTRLEN]; // String representation of address
					inet_ntop(AF_INET6, &(rs.nsaddr_list[i].sin_addr.s_addr), str, INET6_ADDRSTRLEN);
//                     printf("%s\n", str);
                    dns.push_back(boost::asio::ip::address::from_string(str));
				}
			}
#endif	//	__MUSL__
			return dns;
		}
#endif


#if defined(BOOST_OS_WINDOWS_AVAILABLE)

		/**
		 * @see https://docs.microsoft.com/en-gb/windows/win32/api/iphlpapi/nf-iphlpapi-getnetworkparams?redirectedfrom=MSDN
		 */
		std::vector<boost::asio::ip::address>	get_dns_servers()
		{
			std::vector<boost::asio::ip::address> result;

			FIXED_INFO* pFixedInfo;
			ULONG ulOutBufLen;
			DWORD dwRetVal;
			IP_ADDR_STRING* pIPAddr;

			pFixedInfo = (FIXED_INFO*)DNS_MALLOC(sizeof(FIXED_INFO));
			if (pFixedInfo == NULL) {
				//printf("Error allocating memory needed to call GetNetworkParams\n");
				return result;
			}
			ulOutBufLen = sizeof(FIXED_INFO);

			// Make an initial call to GetAdaptersInfo to get
			// the necessary size into the ulOutBufLen variable
			if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
				DNS_FREE(pFixedInfo);
				pFixedInfo = (FIXED_INFO*)DNS_MALLOC(ulOutBufLen);
				if (pFixedInfo == NULL) {
					//printf("Error allocating memory needed to call GetNetworkParams\n");
					return result;
				}
			}

			if (dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen) == NO_ERROR) {

				result.push_back(boost::asio::ip::address::from_string(pFixedInfo->DnsServerList.IpAddress.String));

				pIPAddr = pFixedInfo->DnsServerList.Next;
				while (pIPAddr) {
					result.push_back(boost::asio::ip::address::from_string(pIPAddr->IpAddress.String));
					pIPAddr = pIPAddr->Next;
				}
			}

			if (pFixedInfo) DNS_FREE(pFixedInfo);

			return result;
		}
#endif

	}
}




