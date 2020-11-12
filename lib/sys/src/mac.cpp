/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/mac.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#elif BOOST_OS_LINUX

#include <cyng/parser/mac_parser.h>
#include <fstream> 
#include <cyng/compatibility/file_system.hpp>
#else
#warning unknow OS
#endif

namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX
        std::vector<mac48> retrieve_mac48(std::string const& name)
		{
			// sol@paramount:~> cat /sys/class/net/eno16777736/address
			// 00:0c:29:cc:e3:d4

			filesystem::path const root("/sys/class/net/");
			filesystem::path const p = root / name / "address";
            std::vector<mac48>	result;

			//	open file
			std::ifstream infile(p.string(), std::ios::in);

			//	read line by line
			std::string line;
			if (std::getline(infile, line, '\n'))
			{
// 				std::cout << "parse: " << line << std::endl;
                const std::pair<cyng::mac48, bool > r = cyng::parse_mac48(line);
				if (r.second && !r.first.is_nil())
				{
                    result.push_back(r.first);
				}
			}

            return result;
		}
#endif


		std::vector<mac48>	retrieve_mac48()
		{
			std::vector<mac48> result;
#if BOOST_OS_WINDOWS

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
					result.push_back(mac48(pAdapterInfo->PhysicalAddress[0]
						, pAdapterInfo->PhysicalAddress[1]
						, pAdapterInfo->PhysicalAddress[2]
						, pAdapterInfo->PhysicalAddress[3]
						, pAdapterInfo->PhysicalAddress[4]
						, pAdapterInfo->PhysicalAddress[5]));

					pAdapterInfo = pAdapterInfo->Next;                      // Progress through linked list
				};
			}
#elif BOOST_OS_LINUX

			//		ls /sys/class/net/
			const filesystem::path p("/sys/class/net/");
			BOOST_ASSERT_MSG(filesystem::is_directory(p), "not a directory");

			std::for_each(filesystem::directory_iterator(p)
				, filesystem::directory_iterator()
				, [&result](filesystem::path const& adapter)
			{
				BOOST_ASSERT_MSG(filesystem::is_directory(adapter), "not a directory");
				if (filesystem::is_directory(adapter))
				{
					const filesystem::path name = adapter.stem();
                    const std::vector<mac48> al = retrieve_mac48(name.string());
                    result.insert(result.end(), al.begin(), al.end());
				}
			});

#else
			#warning unknow OS
#endif
			return result;
		}


	}
}




