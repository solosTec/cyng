/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_INFO_H
#define CYNG_SYS_INFO_H

#include <string>
#include <chrono>

namespace cyng 
{
	namespace sys 
	{
		/**
		 * Get operating system name (e.g. Linux)
		 */
		std::string	get_os_name();
		std::string	get_os_release();

	}
}

#endif
