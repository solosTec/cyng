#include <cyng/sys/cpu.h>

#include <boost/predef.h>
#if defined(BOOST_OS_WINDOWS_AVAILABLE)

#include <windows.h>
#include "TCHAR.h"
#include "pdh.h"
#pragma comment(lib, "pdh.lib")

#elif defined(BOOST_OS_LINUX_AVAILABLE)

#include <cyng/io/ostream.h>
#include <cyng/parse/string.h>

#include "sys/types.h"
#include "sys/sysinfo.h"
#include <fstream> 
#include <string> 
#include <cctype>
#include <iostream>
#include <numeric>

//#include <boost/algorithm/string.hpp>

#else
#warning unknow OS
#endif

/*
 * on linux system /proc/meminfo provides the following information:
 * 
cpu  90468 903 14200 2315085 2875 0 1052 0 0 0
cpu0 22911 269 3555 578754 872 0 311 0 0 0
cpu1 21704 207 3806 579817 581 0 248 0 0 0
cpu2 22369 274 3860 578107 792 0 208 0 0 0
cpu3 23482 152 2978 578406 628 0 283 0 0 0
intr 4214383 2 4848 0 0 0 0 0 0 1 0 0 0 106479 0 0 6350 33358 87036 200 31381 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 861 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
ctxt 9613040
btime 1616782159
processes 4761
procs_running 1
procs_blocked 0
softirq 1955498 1 244763 1236 31463 96070 0 4587 710324 0 867054

cu = 2.424.583 = 100%
idle = 2315085
*/

namespace cyng {
	namespace sys
	{

#if defined(BOOST_OS_LINUX_AVAILABLE)

		enum cpu_info {
			USER,
			NICE,
			SYSTEM,
			IDLE,
			IOWAIT,
			IRQ,
			SOFTIRQ,
			STEAL,	//!< Since Linux 2.6.11
			GUEST,	//!< Since Linux 2.6.24	
			GUEST_NICE	//!< Since Linux 2.6.33	
		};

		namespace {
			/**
			 * Extract CPU values from a system file like "/proc/stat.
			 * example:
			 * cpu  92188 1300 23758 35845024 2289 0 1930 0 0 0
			 * cpu0 22068 300 5792 8966142 436 0 382 0 0 0
			 * cpu1 23587 356 5993 8959443 590 0 312 0 0 0
			 * cpu2 24480 264 6352 8955013 680 0 286 0 0 0
			 * cpu3 22052 380 5620 8964425 582 0 948 0 0 0
			 * intr ...
			 */
			std::array<std::uint64_t, 10> read_cpu_stat(std::size_t core) {
				std::ifstream ifs("/proc/stat");
				BOOST_ASSERT(ifs.is_open());

				std::size_t counter{ 0 };
				std::array<std::uint64_t, 10> r{ 0 };
				std::string name;
				while (ifs) {
					ifs >> name;
					if (boost::algorithm::starts_with(name, "cpu")) {
						for(auto& v : r) {
							ifs >> v;
						}
						if (counter == core)	return r;
					}
					else {
						return r;
					}
				}
				return r;
			}
		}
#endif

#if defined(BOOST_OS_WINDOWS_AVAILABLE)
		void init(PDH_HQUERY& cpu_query, PDH_HCOUNTER* cpu_total)
		{
			::PdhOpenQuery(NULL, NULL, &cpu_query);
			// You can also use L"\\Processor(*)\\% Processor Time" and get individual CPU values with PdhGetFormattedCounterArray()
			::PdhAddEnglishCounter(cpu_query, "\\Processor(_Total)\\% Processor Time", NULL, cpu_total);
			::PdhCollectQueryData(cpu_query);
		}
#endif

		double get_cpu_load(std::size_t core) {
#if defined(BOOST_OS_WINDOWS_AVAILABLE)

			static PDH_HQUERY cpu_query{ nullptr };
			static PDH_HCOUNTER cpu_total{ nullptr };
			if (cpu_query == nullptr) {
				init(cpu_query, &cpu_total);
			}

			PDH_FMT_COUNTERVALUE counterVal;

			::PdhCollectQueryData(cpu_query);
			::PdhGetFormattedCounterValue(cpu_total, PDH_FMT_DOUBLE, NULL, &counterVal);
			return counterVal.doubleValue;

#elif defined(BOOST_OS_LINUX_AVAILABLE)
			//
			//	to calculate a utilization the delta to the previous
			//	readout must be computed.
			//
			static std::size_t prev_idle_time{ 0 }, prev_total_time{ 0 };

			auto const r = read_cpu_stat(core);
			//	yields 100% CPU time
			auto const total = std::accumulate(std::begin(r), std::end(r), 0);
			BOOST_ASSERT(r.at(IDLE) < total);

			// std::size_t counter{ 0 };
			// for(auto& v : r) {
			// 	if (counter == IDLE) {
			// 		std::cout << "[" << v << "] ";
			// 	}
			// 	else {
			// 		std::cout << v << " ";
			// 	}
			// 	++counter;
			// }
			// std::cout << " total: " << total << std::endl;

			//	calculate utilization
			//	force to floating point division
			const double idle_time_delta = r.at(IDLE) - prev_idle_time;
			const double total_time_delta = total - prev_total_time;
			// const double utilization = 100.0 * (1.0 - idle_time_delta / total_time_delta);
			const double utilization = 1.0 - (idle_time_delta / total_time_delta);

			prev_idle_time = r.at(IDLE);
			prev_total_time = total;	
			return utilization;
#else
			return 0.0;
#endif

		}
	}
}
