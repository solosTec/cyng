/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
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
			return std::vector<std::string>{};
		}
#endif


#if BOOST_OS_WINDOWS
		std::vector<std::string> get_ntp_servers()
		{
			//	Computer\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\DateTime\Servers
			unsigned idx{ 0 };
			std::vector<std::string> result;
			while (true) {

				registry_string<std::string> req("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\DateTime\\Servers", idx, HKEY_LOCAL_MACHINE);

				if (req.exists()) {
					result.push_back(req);
					++idx;
				}
				else {
					break;
				}
			}
			return result;
		}
#endif

	}
}




