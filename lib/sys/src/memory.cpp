/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/memory.h>
#include <boost/predef.h>
// #include <boost/assert.hpp>

#if BOOST_OS_WINDOWS

#include "windows.h"
#include <string> 
#include <cctype>
#include <iostream>

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
		//	Total Virtual Memory
		std::uint64_t get_total_virtual_memory()
		{
#if BOOST_OS_WINDOWS
			return 0;
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
			return 0;
#elif BOOST_OS_LINUX			
			struct sysinfo memInfo;
			sysinfo (&memInfo);
			std::uint64_t virtualMemUsed = memInfo.totalram - memInfo.freeram;
			//Add other values in next statement to avoid int overflow on right hand side...
			virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
			virtualMemUsed *= memInfo.mem_unit;
			return virtualMemUsed;
#else
			return 0;
#endif
		}
		
		std::uint64_t get_total_physical_memory()
		{
#if BOOST_OS_WINDOWS
			return 0;
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
			return 0;
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

		std::uint64_t get_used_virtual_memory_by_process()
		{
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
		}
#endif

	}
}


