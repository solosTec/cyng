/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/info.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include <windows.h>
#include <sstream>

#elif BOOST_OS_LINUX

#include <sys/utsname.h>

#else
#warning unknow OS
#endif

namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX
		std::string	get_os_name()
		{
			utsname info;
			return (::uname(&info) == 0)
				? info.sysname
				: ""
				;
		}
		
		std::string	get_os_release()
		{
			utsname info;
			return (::uname(&info) == 0)
			? info.release
			: ""
			;
		}
#endif


#if BOOST_OS_WINDOWS
		std::string	get_os_name()
		{
			return "windows";
		}
		
		std::string	get_os_release()
		{
			OSVERSIONINFOW info;
			::ZeroMemory(&info, sizeof(OSVERSIONINFOW));
			info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOW);
			
			LPOSVERSIONINFOW lp_info = &info;
			::GetVersionEx(lp_info);
			
			std::stringstream ss;
			ss << info.dwMajorVersion << '.' << info.dwMinorVersion;
			
			return ss.str();
		}
#endif


	}
}




