#include <cyng/sys/memory.h>

#include <boost/predef.h>
#if defined(BOOST_OS_WINDOWS_AVAILABLE)

#include <cyng/sys/windows.hpp>
#include <psapi.h>

#include <string> 
#include <cctype>
#include <cyng/io/ostream.h>
#pragma comment(lib, "Psapi.lib")

#elif defined(BOOST_OS_LINUX_AVAILABLE)

#include "sys/types.h"
#include "sys/sysinfo.h"
#include <fstream> 
#include <string> 
#include <cctype>
#include <cyng/io/ostream.h>


#else
#warning unknow OS
#endif


namespace cyng {
	namespace sys
	{
		std::uint64_t get_total_ram() {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPhys;	//	ullTotalPhys

#elif defined(BOOST_OS_LINUX_AVAILABLE)
			struct sysinfo memInfo;
			sysinfo(&memInfo);
			//Multiply in next statement to avoid int overflow on right hand side...
			return static_cast<std::uint64_t>(memInfo.totalram) * memInfo.mem_unit;
#else
			return 0;
#endif

		}

		std::uint64_t get_used_ram() {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPhys - memInfo.ullAvailPhys;

#elif defined(BOOST_OS_LINUX_AVAILABLE)
			struct sysinfo memInfo;
			sysinfo(&memInfo);
			//Multiply in next statement to avoid int overflow on right hand side...
			return (static_cast<std::uint64_t>(memInfo.totalram) - memInfo.freeram) * memInfo.mem_unit;
#else
			return 0;
#endif

		}


		std::uint64_t get_total_virtual_memory() {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalVirtual;

#elif defined(BOOST_OS_LINUX_AVAILABLE)
			struct sysinfo memInfo;
			sysinfo(&memInfo);
			//	Add other values in next statement to avoid int overflow on right hand side...
			return (static_cast<std::uint64_t>(memInfo.totalram) + memInfo.totalswap) * memInfo.mem_unit;
#else
			return 0;
#endif

		}

		std::uint64_t get_used_virtual_memory() {
#if BOOST_OS_WINDOWS

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

#elif BOOST_OS_LINUX			
			struct sysinfo mem_info;
			sysinfo(&mem_info);
			//Add other values in next statement to avoid int overflow on right hand side...
			return ((static_cast<std::uint64_t>(memInfo.totalram) - memInfo.freeram) + (memInfo.totalswap - memInfo.freeswap)) * memInfo.mem_unit;
#else
			return 0;
#endif

		}

	}
}
