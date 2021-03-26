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

#include <cyng/io/ostream.h>
#include <cyng/parse/string.h>

#include "sys/types.h"
#include "sys/sysinfo.h"
#include <fstream> 
#include <string> 
#include <cctype>

#include <boost/algorithm/string.hpp>

#else
#warning unknow OS
#endif

/*
 * on linux system /proc/meminfo provides the following information:
 * 
 *	MemTotal:        8123340 kB
 *	MemFree:         5788504 kB
 *	MemAvailable:    6631084 kB
 *	Buffers:           63488 kB
 *	Cached:           946576 kB
 *	SwapCached:            0 kB
 *	Active:          1237968 kB
 *	Inactive:         551416 kB
 *	Active(anon):     779556 kB
 *	Inactive(anon):     1984 kB
 *	Active(file):     458412 kB
 *	Inactive(file):   549432 kB
 *	Unevictable:          16 kB
 *	Mlocked:              16 kB
 *	SwapTotal:       2097148 kB
 *	SwapFree:        2097148 kB
 *	Dirty:               132 kB
 *	Writeback:             0 kB
 *	AnonPages:        779340 kB
 *	Mapped:           304084 kB
 *	Shmem:              3380 kB
 *	KReclaimable:     111904 kB
 *	Slab:             251212 kB
 *	SReclaimable:     111904 kB
 *	SUnreclaim:       139308 kB
 *	KernelStack:       10960 kB
 *	PageTables:        12656 kB
 *	NFS_Unstable:          0 kB
 *	Bounce:                0 kB
 *	WritebackTmp:          0 kB
 *	CommitLimit:     6158816 kB
 *	Committed_AS:    3724580 kB
 *	VmallocTotal:   34359738367 kB
 *	VmallocUsed:       25784 kB
 *	VmallocChunk:          0 kB
 *	Percpu:           101376 kB
 *	HardwareCorrupted:     0 kB
 *	AnonHugePages:         0 kB
 *	ShmemHugePages:        0 kB
 *	ShmemPmdMapped:        0 kB
 *	FileHugePages:         0 kB
 *	FilePmdMapped:         0 kB
 *	HugePages_Total:       0
 *	HugePages_Free:        0
 *	HugePages_Rsvd:        0
 *	HugePages_Surp:        0
 *	Hugepagesize:       2048 kB
 *	Hugetlb:               0 kB
 *	DirectMap4k:      261952 kB
 *	DirectMap2M:     6029312 kB
 *	DirectMap1G:     4194304 kB
*/

namespace cyng {
	namespace sys
	{

#if defined(BOOST_OS_LINUX_AVAILABLE)

		/**
		 * Extract values from a system file like "/proc/uptime.
		 * You can read other files too.
		 */
		std::map<std::string, std::uint64_t> read_mem_info() {
			std::ifstream ifs("/proc/meminfo");

			std::map<std::string, std::uint64_t> r;
			std::string name, unit;
			std::uint64_t value;
			while (ifs) {
				ifs >> name >> value >> unit;
				if (boost::algorithm::equals(unit, "kb")) {
					r.emplace(name, value * 1024u);
				}
				else {
					r.emplace(name, value);
				}
			}
			return r;
		}

#endif

		std::uint64_t get_total_ram() {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPhys;	//	ullTotalPhys

#elif defined(BOOST_OS_LINUX_AVAILABLE)
			//MemTotal:        8123340 kB	<==
			//MemFree:         5788504 kB
			//MemAvailable:    6631084 kB
			auto const info = read_mem_info();
			auto const pos = info.find("MemTotal:");
			return (pos != info.end())
				? pos->second
				: 0u
				;
#elif defined(BOOST_OS_LINUX_AVAILABLE_ALTERNATE)
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
			//MemTotal:        8123340 kB
			//MemFree:         5788504 kB
			//MemAvailable:    6631084 kB
			auto const info = read_mem_info();
			auto const pos_total = info.find("MemTotal:");
			auto const pos_avail = info.find("MemAvailable:");
			return (pos_total != info.end() && pos_avail != info.end())
				? (pos_total->second - pos_avail->second)
				: 0u
				;
#elif defined(BOOST_OS_LINUX_AVAILABLE_ALTERNATE)
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

			struct sysinfo memInfo;
			sysinfo(&memInfo);
			//Add other values in next statement to avoid int overflow on right hand side...
			return ((static_cast<std::uint64_t>(memInfo.totalram) - memInfo.freeram) + (memInfo.totalswap - memInfo.freeswap)) * memInfo.mem_unit;
#else
			return 0;
#endif

		}

	}
}
