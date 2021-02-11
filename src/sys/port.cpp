#include <cyng/sys/port.h>
#include <boost/predef.h>

#if defined(BOOST_OS_WINDOWS_AVAILABLE)

#include <Windows.h>
#include <Setupapi.h>
#pragma comment(lib, "Setupapi.lib")

#else

#include <cyng/sys/linux.hpp>
#include <filesystem>

#endif

#include <iostream>

namespace cyng {
	namespace sys {

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
		void get_serial_ports(std::vector<std::string>& vec)	{

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
#ifdef _DEBUG_SYS
				std::cout << "**error : SetupDiClassGuidsFromName() failed..." << std::endl;
#endif
				return;
			}

			hdevinfo = SetupDiGetClassDevs(&guid[0], NULL, NULL, DIGCF_PRESENT | DIGCF_PROFILE);
			if (hdevinfo == INVALID_HANDLE_VALUE) {
#ifdef _DEBUG_SYS
				std::cout << "**error : SetupDiGetClassDevs() failed..." << std::endl;
#endif
				return;
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
#ifdef _DEBUG_SYS
					std::cout << "**warning : SetupDiGetDeviceRegistryProperty() failed..." << std::endl;
#endif
					continue;
				}

				hKey = ::SetupDiOpenDevRegKey(hdevinfo, &devinfo_data, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
				if (!hKey) continue;

				size = sizeof(port_name);
				rv = ::RegQueryValueEx(hKey, "PortName", 0, &type, (LPBYTE)&port_name, &size);
				vec.push_back(port_name);

				//registry_string<std::string> reg("PortName", 0, hKey);
				//result.push_back(reg);

				::RegCloseKey(hKey);
			}

			SetupDiDestroyDeviceInfoList(hdevinfo);

		}
#else
		void get_serial_ports(std::vector<string>& vec) {
			filesystem::path const d("/sys/class/tty");
			if (filesystem::is_directory(d)) {
				for (filesystem::directory_entry const& entry : filesystem::directory_iterator(d)) {
					//
					//	test if /sys/class/tty/"entry"/device exists
					//
					auto const dev = entry.path() / "device";
					if (filesystem::exists(dev)) {
						vec.push_back(entry.path().filename().string());
					}
				}
			}

		}
#endif

		std::vector<std::string> get_serial_ports()	{

			std::vector<std::string>  result;
			get_serial_ports(result);
			return result;
		}
	}
}
