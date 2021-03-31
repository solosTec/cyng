#include <cyng/sys/mac.h>
#include <boost/predef.h>

#if defined(BOOST_OS_WINDOWS_AVAILABLE)

#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#else

#include <cyng/sys/linux.hpp>
#include <cyng/parse/mac.h>
#include <filesystem>

#endif

#include <iostream>

namespace cyng {
	namespace sys {

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
		void get_mac48_adresses(std::vector<mac48>& vec)	{

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
					vec.push_back(mac48(pAdapterInfo->PhysicalAddress[0]
						, pAdapterInfo->PhysicalAddress[1]
						, pAdapterInfo->PhysicalAddress[2]
						, pAdapterInfo->PhysicalAddress[3]
						, pAdapterInfo->PhysicalAddress[4]
						, pAdapterInfo->PhysicalAddress[5]));

					pAdapterInfo = pAdapterInfo->Next;                      // Progress through linked list
				};
			}
		}
#else
        std::vector<mac48> get_mac48(std::string const& name)	{
			// > cat /sys/class/net/eno16777736/address
			// 00:0c:29:cc:e3:d4

			auto const root = std::filesystem::path("/sys/class/net/");
			auto const p = root / name / "address";
            std::vector<mac48>	result;

			//	open file
			std::ifstream infile(p.string(), std::ios::in);

			//	read line by line
			std::string line;
			if (std::getline(infile, line, '\n'))
			{
 				//std::cout << "parse: " << line << std::endl;
                auto const mac = to_mac48(line);
				//
				//	skip value from loopback device
				//
				if (!is_nil(mac))	{
                    result.push_back(mac);
				}
			}

            return result;
		}

		void get_mac48_adresses(std::vector<mac48>& vec)	{

			auto const p = std::filesystem::path("/sys/class/net/");
			BOOST_ASSERT_MSG(std::filesystem::is_directory(p), "not a directory");

			std::for_each(std::filesystem::directory_iterator(p)
				, std::filesystem::directory_iterator()
				, [&vec](std::filesystem::path const& adapter)
			{
				BOOST_ASSERT_MSG(std::filesystem::is_directory(adapter), "not a directory");
				if (std::filesystem::is_directory(adapter))
				{
					auto const name = adapter.stem();
                    auto const al = get_mac48(name.string());
                    vec.insert(vec.end(), al.begin(), al.end());
				}
			});

		}
#endif

		std::vector<mac48> get_mac48_adresses()	{

			std::vector<mac48>  result;
			get_mac48_adresses(result);
			return result;
		}
	}
}
