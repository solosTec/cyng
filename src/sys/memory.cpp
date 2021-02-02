#include <cyng/sys/memory.h>

#include <boost/predef.h>
#if BOOST_OS_WINDOWS

#include <Windows.h>
#include <psapi.h>

#include <string> 
#include <cctype>
#include <cyng/io/ostream.h>
#pragma comment(lib, "Psapi.lib")

#elif BOOST_OS_LINUX

#include "sys/types.h"
#include "sys/sysinfo.h"
#include <fstream> 
#include <string> 
#include <cctype>
#include <cyng/io/ostream.h>


#else
#warning unknow OS
#endif


namespace docscript {
	namespace sys
	{
		std::uint64_t get_total_physical_memory() {
#if BOOST_OS_WINDOWS

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return static_cast<std::uint64_t>(memInfo.ullTotalPhys);	//	ullAvailPhys

#elif BOOST_OS_LINUX			
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
