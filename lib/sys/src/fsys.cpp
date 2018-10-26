/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/sys/fsys.h>
#include <boost/predef.h>
#include <boost/assert.hpp>
#include <iostream>

#if BOOST_OS_WINDOWS

#include <windows.h>

#elif BOOST_OS_LINUX

#include <fstream> 
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>	//	is_any_of

#else
#warning unknow OS
#endif

namespace cyng 
{
	namespace sys 
	{
#if BOOST_OS_LINUX
		std::vector<std::string> get_drive_names()
		{
			std::vector<std::string> drives;
			
			//
			//	get all drives by reading /etc/fstab
			//
			std::ifstream ifs("/etc/fstab");
			
			//
			//	example: LABEL=t-home2   /home      ext4    defaults,auto_da_alloc      0  2
			//	
			//	1 - block special device or remote filesystem to be mounted
			//	2 - mount point (target)
			//	3 - type of the filesystem
			//	4 - type of the filesystem
			//	5 - filesystems need to be dumped
			//	6 - checks at boot time.
			//
			std::string line;
			while (std::getline(ifs, line)) {
				// line contains the current line
				std::vector<std::string> data;
				boost::algorithm::split(data, line, boost::algorithm::is_any_of(" \t"), boost::algorithm::token_compress_on);
				if (data.size() == 6) {
					drives.push_back(data.at(1));
				}
			}			
			return drives;
		}
#else
		std::vector<std::string> get_drive_names()
		{
			std::vector<std::string> drives;
			
			char buffer[BUFSIZE];
			buffer[0] = '\0';

			if (::GetLogicalDriveStrings(BUFSIZE-1, buffer)) {
				
				char* p = buffer;
				
				// Go to the next NULL character.
//  			while (*p++);
			}
			
			return drives;
		}
#endif
	}
}




