#include <cyng/sys/mac.h>
#include <boost/predef.h>

#if defined(BOOST_OS_WINDOWS_AVAILABLE)

#include <cyng/sys/windows.h>

#else

#include <cyng/sys/linux.hpp>
#include <cyng/parse/mac.h>
#include <filesystem>

#endif

#include <iostream>
#include <boost/algorithm/string.hpp>

namespace cyng {
	namespace sys {

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
		namespace {	//	static linkage

			void read_unspec_info(ip_address_cb cb) {
				//
				//	read adapter info
				//
				auto pAdapterInfo = get_adapter_adresses(AF_UNSPEC);

				//
				//	exit loop if callback function returns false
				//
				while ((pAdapterInfo != nullptr) && cb(*pAdapterInfo, convert_to_utf8(pAdapterInfo->FriendlyName))) {
					//
					//	next address
					//
					pAdapterInfo = pAdapterInfo->Next;
				}

				//
				//	free memory
				//
				free(pAdapterInfo);
			}
		}

		mac48 get_mac48(std::string device) {
			mac48 r;
			read_unspec_info([&](IP_ADAPTER_ADDRESSES const& address, std::string name)->bool {
				if (address.IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
					if (boost::algorithm::equals(device, name)) {
						r = mac48(address.PhysicalAddress[0]
							, address.PhysicalAddress[1]
							, address.PhysicalAddress[2]
							, address.PhysicalAddress[3]
							, address.PhysicalAddress[4]
							, address.PhysicalAddress[5]);
						return false;
					}
				}
				return true;
				}
			);
			return r;
		}

		void get_mac48_adresses(std::vector<mac48>& vec)	{

			read_unspec_info([&](IP_ADAPTER_ADDRESSES const& address, std::string name)->bool {
				if (address.IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
					vec.push_back(mac48(address.PhysicalAddress[0]
						, address.PhysicalAddress[1]
						, address.PhysicalAddress[2]
						, address.PhysicalAddress[3]
						, address.PhysicalAddress[4]
						, address.PhysicalAddress[5]));
				}
				return true;
				}
			);

		}
#else
        mac48 get_mac48(std::string const& name)	{
			// > cat /sys/class/net/eno16777736/address
			// 00:0c:29:cc:e3:d4

			auto const root = std::filesystem::path("/sys/class/net/");
			auto const p = root / name / "address";

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
                    return mac;
				}
			}

			return mac48{};
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
                    vec.push_back(get_mac48(name.string()));
				}
			});

		}
#endif

		std::vector<mac48> get_mac48_adresses()	{

			std::vector<mac48>  result;
			get_mac48_adresses(result);
			return result;
		}

		mac48 get_mac48_adress(std::string device) {
			return get_mac48(device);
		}

	}
}
