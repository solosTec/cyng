/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/sys/info.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include <cyng/scm/win_registry.h>
#include <VersionHelpers.h>
#include <sstream>

#elif BOOST_OS_LINUX

#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <fstream>

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
			//
			// read file /proc/version
			//
			std::filesystem::path const p{"proc/version"};
			
			//	open file
			std::ifstream infile(p.string(), std::ios::in);
			
			//	read line by line
			std::string line;
			if (std::getline(infile, line)) {
				return line;
			}


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

															registry_string<std::string> req("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "CurrentBuild", HKEY_LOCAL_MACHINE);
															if (req.exists()) {
																ss << " build " << req.operator std::string();
															}

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
			//	--- approach (1)

			OSVERSIONINFO info;
			::ZeroMemory(&info, sizeof(OSVERSIONINFO));
			info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			
			::GetVersionEx(&info);

			std::stringstream ss;
			ss << info.dwMajorVersion << '.' << info.dwMinorVersion;

			return ss.str();

			//	--- approach (2)

			//OSVERSIONINFOEXW info;
			//::RtlGetVersion(&info);
			//
			//std::stringstream ss;
			//ss << info.dwMajorVersion << '.' << info.dwMinorVersion << '.' << info.dwBuildNumber;

			//return ss.str();

			//	--- approach (3)

			//::GetProductInfo(
			//	DWORD  dwOSMajorVersion,
			//	DWORD  dwOSMinorVersion,
			//	DWORD  dwSpMajorVersion,
			//	DWORD  dwSpMinorVersion,
			//	PDWORD pdwReturnedProductType
			//);
		}
#endif

	}
}




