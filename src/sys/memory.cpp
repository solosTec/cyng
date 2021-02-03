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
			return static_cast<std::uint64_t>(memInfo.ullTotalPhys);	//	ullAvailPhys

#elif defined(BOOST_OS_LINUX_AVAILABLE)
			struct sysinfo memInfo;
			sysinfo(&memInfo);
			//Multiply in next statement to avoid int overflow on right hand side...
			return memInfo.totalram * memInfo.mem_unit;
#else
			return 0;
#endif

		}
	}
}
