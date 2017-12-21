/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/cpu.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include "windows.h"

#elif BOOST_OS_LINUX

// #include "sys/types.h"
// #include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/times.h"
#include "sys/vtimes.h"
#include <cstdlib>
#include <fstream> 
#include <array>
#include <cstdint>
#include <numeric>
#else
#warning unknow OS
#endif

namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX
        using data_t = std::array<std::size_t, 10>;
        
		/**
		 * For reasons I don't understood yet, this function returns
		 * always the same values after the first read when used 
		 * in a shared library.
		 */
        data_t get_values()
        {
            data_t data{ 0 };
            std::string tmp;
			std::ifstream ifs("/proc/stat");
            
            //
            //  1 - user: normal processes executing in user mode
            //  2 - niced processes executing in user mode
            //  3 - system: processes executing in kernel mode
            //  4 - idle: twiddling thumbs
            //  5 - iowait: waiting for I/O to complete
			//	6 - irq: time servicing hardware interrupts
			//	7 - softirq: time servicing software interrupts
			//	8 - steal: time spent in other operating systems when running in a virtualized environment
			//	9 - guest: time spent running a virtual CPU for guest operating systems
			//	10 - guest_nice: time spent running a low priority virtual CPU for guest operating systems
            ifs >> tmp; //  cpu
            BOOST_ASSERT(tmp == "cpu");
            for (auto& n : data)
            {
                ifs >> n;
            }
            ifs.close();	//	explicit
            return data;
        }        
#endif

		double get_total_cpu_load()
		{
#if BOOST_OS_WINDOWS
			return 0;
#elif BOOST_OS_LINUX
            
//             static data_t last_data{ 0 };
//             static std::uint64_t last_idle{ 0 }, last_total{ 0 };
			static std::size_t previous_idle_time{ 0 }, previous_total_time{ 0 };

            const data_t new_data = get_values();

			//	get idle time
            std::size_t idle = new_data[3];
        
            //  calulate the total CPU time.
            std::size_t total = std::accumulate(new_data.begin(), new_data.end(), 0);
        
			//	calculate utilization
			const float idle_time_delta = idle - previous_idle_time;
			const float total_time_delta = total - previous_total_time;
			const float utilization = 100.0 * (1.0 - idle_time_delta / total_time_delta);
			
			previous_idle_time = idle;
			previous_total_time = total;			
            return utilization;
                
#else
			return 0.0;
#endif
		}
		
		std::uint64_t get_cpu_load_by_process()
		{
#if BOOST_OS_WINDOWS
			return 0;
#elif BOOST_OS_LINUX			
			return (std::rand() % 100);
#else
			return 0;
#endif
		}
	}
}




