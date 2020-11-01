/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/memory.h>
#include <boost/predef.h>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/core/ignore_unused.hpp>

#if BOOST_OS_WINDOWS

#include "windows.h"
#include "psapi.h"
#include <string> 
#include <cctype>
#include <iostream>
#pragma comment(lib, "Psapi.lib")

#elif BOOST_OS_LINUX

#include "sys/types.h"
#include "sys/sysinfo.h"
#include <fstream> 
#include <string> 
#include <cctype>
#include <iostream>

#else
#warning unknow OS
#endif
	
//	see https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX	
        int parse_line(std::string const& line)
		{
			// This assumes that a digit will be found and the line ends in " Kb".
// 			VmRSS:	     784 kB
// 			std::cout << line << std::endl;
			std::string val;
			for (auto c : line)
			{
				if (std::isdigit(static_cast<unsigned char>(c)))
				{
					val += c;
				}
			}
			return std::stoi(val);
		}
#endif
        
		//	Total Virtual Memory
		std::uint64_t get_total_virtual_memory()
		{
#if BOOST_OS_WINDOWS

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPageFile;

#elif BOOST_OS_LINUX			
			struct sysinfo memInfo;
			sysinfo (&memInfo);
			std::uint64_t totalVirtualMem = memInfo.totalram;
			//Add other values in next statement to avoid int overflow on right hand side...
			totalVirtualMem += memInfo.totalswap;
			totalVirtualMem *= memInfo.mem_unit;
			return totalVirtualMem;
#else
			return 0;
#endif
		}
		
		std::uint64_t get_used_virtual_memory()
		{
#if BOOST_OS_WINDOWS

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

#elif BOOST_OS_LINUX			
			struct sysinfo mem_info;
			sysinfo (&mem_info);
			std::uint64_t total_virtual_mem = mem_info.totalram;
			total_virtual_mem *= mem_info.mem_unit;
			total_virtual_mem += mem_info.totalswap;
			return total_virtual_mem;
#else
			return 0;
#endif
		}
		
		std::uint8_t get_used_virtual_memory_in_percent()
		{
			try {
				auto const total = cyng::sys::get_total_virtual_memory();
				return (total == 0)
					? 100u
					: boost::numeric_cast<std::uint8_t>(((cyng::sys::get_used_virtual_memory() * 100) / total))
					;
			}
			catch (std::exception const& ex) {
				//numeric::bad_numeric_cast
#ifdef _DEBUG
				std::cerr << "get_used_virtual_memory_in_percent - " << ex.what() << std::endl;
#else
				boost::ignore_unused(ex);
#endif
			}
			return 0u;
		}

		std::uint64_t get_used_virtual_memory_by_process()
		{
#if BOOST_OS_WINDOWS

			PROCESS_MEMORY_COUNTERS_EX pmc;
			ZeroMemory(&pmc, sizeof(PROCESS_MEMORY_COUNTERS_EX));
			::GetProcessMemoryInfo(::GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));
			return pmc.PrivateUsage;

#elif BOOST_OS_LINUX
            
			std::ifstream ifs("/proc/self/status");
			while (ifs)
			{
				std::string line;
				std::getline(ifs, line);
				if (line.compare(0, 7, "VmSize:") == 0) {
					return parse_line(line);
				}
			}
			return 0;

#else
			return 0;
#endif
		}

		std::uint64_t get_total_physical_memory()
		{
#if BOOST_OS_WINDOWS

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPhys;

#elif BOOST_OS_LINUX			
			struct sysinfo memInfo;
			sysinfo (&memInfo);
			std::uint64_t totalPhysMem = memInfo.totalram;
			//Multiply in next statement to avoid int overflow on right hand side...
			totalPhysMem *= memInfo.mem_unit;	
			return totalPhysMem;
#else
			return 0;
#endif
		}
		
		std::uint64_t get_used_physical_memory()
		{
#if BOOST_OS_WINDOWS

			MEMORYSTATUSEX memInfo;
			memInfo.dwLength = sizeof(MEMORYSTATUSEX);
			GlobalMemoryStatusEx(&memInfo);
			return memInfo.ullTotalPhys - memInfo.ullAvailPhys;

#elif BOOST_OS_LINUX			
			struct sysinfo memInfo;
			sysinfo (&memInfo);
			std::uint64_t physMemUsed = memInfo.totalram - memInfo.freeram;
			//Multiply in next statement to avoid int overflow on right hand side...
			physMemUsed *= memInfo.mem_unit;
			return physMemUsed;
#else
			return 0;
#endif
		}
		
		std::uint8_t get_used_physical_memory_in_percent()
		{
#if BOOST_OS_WINDOWS
			try {
				MEMORYSTATUSEX memInfo;
				memInfo.dwLength = sizeof(MEMORYSTATUSEX);
				GlobalMemoryStatusEx(&memInfo);
				return boost::numeric_cast<std::uint8_t>(memInfo.dwMemoryLoad);
			}
			catch (std::exception const& ex) {
#ifdef _DEBUG
				std::cerr << "get_used_physical_memory_in_percent - " << ex.what() << std::endl;
#else
				boost::ignore_unused(ex);
#endif
			}
			return 0;

#elif BOOST_OS_LINUX			

			auto const total = cyng::sys::get_total_physical_memory();
			return (total == 0)
				? 100
				: ((cyng::sys::get_used_physical_memory() * 100) / total)
				;
#else
			return 0;
#endif

		}

#if BOOST_OS_LINUX		
		std::uint64_t get_used_physical_memory_by_process()
		{
			std::ifstream ifs("/proc/self/status");
			while (ifs)
			{
				std::string line;
				std::getline(ifs, line);
				if (line.compare(0, 6, "VmRSS:") == 0) {
					return parse_line(line);
				}
			}
			return 0;
		}

#endif

	}
}



