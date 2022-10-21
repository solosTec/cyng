/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/sys/ntp.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include <cyng/scm/win_registry.h>

#elif BOOST_OS_LINUX


#else
#warning unknow OS
#endif

namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX
		std::vector<std::string> get_ntp_servers()
		{
			std::vector<std::string>	result;
#if defined(CYNG_CROSS_COMPILE)
// 			
			filesystem::path const p{"/usr/local/etc/ntphost"};
			
			//	open file
			std::ifstream infile(p.string(), std::ios::in);
			
			//	read line by line
			std::string line;
			while (std::getline(infile, line))	{
				result.push_back(line);
			}
			
#else
			//	ToDo:
#endif
			return result;
		}
#endif


#if BOOST_OS_WINDOWS
		std::vector<std::string> get_ntp_servers()
		{
			//	Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\DateTime\Servers
			unsigned idx{ 0 };
			std::vector<std::string> result;
			for (unsigned idx{ 0 }; idx < 16; ++idx) {

				registry_string<std::string> req("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\DateTime\\Servers", idx, HKEY_LOCAL_MACHINE);

				if (req.exists()) {
					result.push_back(req);
				}
			}
			return result;
		}
#endif

	}
}




