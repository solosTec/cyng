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
#include <VersionHelpers.h>
#include <sstream>

#elif BOOST_OS_LINUX

#include <sys/utsname.h>
#include <sys/sysinfo.h>

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

			std::stringstream ss;
			ss << "windows ";

			if (::IsWindowsXPOrGreater()) {
				if (::IsWindowsXPSP1OrGreater()) {
					if (::IsWindowsXPSP2OrGreater()) {
						if (::IsWindowsXPSP3OrGreater()) {
							if (::IsWindowsVistaOrGreater()) {
								if (::IsWindowsVistaSP1OrGreater()) {
									if (::IsWindowsVistaSP2OrGreater()) {
										if (::IsWindows7OrGreater()) {
											if (::IsWindows7SP1OrGreater()) {
												if (::IsWindows8OrGreater()) {
													if (::IsWindows8Point1OrGreater()) {
														if (::IsWindows10OrGreater()) {
															ss << "10.x";
														}
														else {
															ss << "8.1";
														}
													}
													else {
														ss << "8.0";
													}
												}
												else {
													ss << "7 SP1";
												}
											}
											else {
												ss << "7";
											}
										}
										else {
											ss << "Vista SP2";
										}
									}
									else {
										ss << "Vista SP1";
									}
								}
								else {
									ss << "Vista";
								}
							}
							else {
								ss << "XP SP3";
							}
						}
						else {
							ss << "XP SP2";
						}
					}
					else {
						ss << "XP SP1";
					}
				}
				else {
					ss << "XP";
				}
			}

			if (::IsWindowsServer()) {
				ss << " server";
			}

			return ss.str();
		}
		
		std::string	get_os_release()
		{
			OSVERSIONINFO info;
			::ZeroMemory(&info, sizeof(OSVERSIONINFO));
			info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			
			::GetVersionEx(&info);
			
			std::stringstream ss;
			ss << info.dwMajorVersion << '.' << info.dwMinorVersion;
			
			return ss.str();
		}
#endif


		std::chrono::milliseconds get_uptime()
		{
#if BOOST_OS_WINDOWS
			return std::chrono::milliseconds(GetTickCount64());
#elif BOOST_OS_LINUX
			struct sysinfo x;
			if (sysinfo(&x) == 0)	{
				//	Seconds since boot
				return std::chrono::milliseconds(static_cast<unsigned long long>(x.uptime) * 1000ULL);
			}
			return std::chrono::milliseconds(0u);
#else
			return std::chrono::milliseconds(0u);
#endif
		}

	}
}




