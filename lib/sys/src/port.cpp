/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2020 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/port.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <cyng/compatibility/file_system.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include <cyng/scm/win_registry.h>
//#include <Windows.h>
#include <Setupapi.h>
#pragma comment(lib, "Setupapi.lib")

#else

#include <fstream> 

#endif

namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX
		std::vector<std::string> get_ports()
		{
			std::vector<std::string>	result;
			//
			//	get all serial interfaces by reading /etc/fstab
			//
// 			std::ifstream ifs("/sys/class/tty");
			filesystem::path const d("/sys/class/tty");
			if (filesystem::is_directory(d)) {
				for (filesystem::directory_entry& entry : filesystem::directory_iterator(d)) {
					//
					//	test if /sys/class/tty/"entry"/device exists
					//
					auto const dev = entry.path() / "device";
					if (filesystem::exists(dev)) {
						result.push_back(entry.path().filename().string());
					}
				}
			}
			return result;
		}
#endif


#if BOOST_OS_WINDOWS
		std::vector<std::string> get_ports()
		{
			std::vector<std::string> result;
			BOOL rv;
			DWORD size;
			GUID guid[1];
			HDEVINFO hdevinfo;
			DWORD idx = 0;
			SP_DEVINFO_DATA devinfo_data;
			devinfo_data.cbSize = sizeof(SP_DEVINFO_DATA);
			int count = 0;

			rv = SetupDiClassGuidsFromName("Ports", (LPGUID)&guid, 1, &size);
			if (!rv) {
				std::cout << "error : SetupDiClassGuidsFromName() failed..." << std::endl;
				return result;
			}

			hdevinfo = SetupDiGetClassDevs(&guid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
			if (hdevinfo == INVALID_HANDLE_VALUE) {
				std::cout << "error : SetupDiGetClassDevs() failed..." << std::endl;
				return result;
			}

			while (SetupDiEnumDeviceInfo(hdevinfo, idx++, &devinfo_data)) {
				char friendly_name[MAX_PATH];
				char port_name[MAX_PATH];
				DWORD prop_type;
				DWORD type = REG_SZ;
				HKEY hKey = NULL;

				rv = ::SetupDiGetDeviceRegistryProperty(hdevinfo, &devinfo_data, SPDRP_FRIENDLYNAME, &prop_type,
					(LPBYTE)friendly_name, sizeof(friendly_name), &size);
				if (!rv) {
					std::cout << "error : SetupDiGetDeviceRegistryProperty() failed..." << std::endl;
					continue;
				}

				hKey = ::SetupDiOpenDevRegKey(hdevinfo, &devinfo_data, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
				if (!hKey) continue;

				size = sizeof(port_name);
				rv = ::RegQueryValueEx(hKey, "PortName", 0, &type, (LPBYTE)&port_name, &size);
				result.push_back(port_name);

				//registry_string<std::string> reg("PortName", 0, hKey);
				//result.push_back(reg);

				::RegCloseKey(hKey);
			}

			SetupDiDestroyDeviceInfoList(hdevinfo);
			return result;
		}
#endif

	}
}




