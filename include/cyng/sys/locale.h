/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_LOCALE_H
#define CYNG_SYS_LOCALE_H

/** @file locale.h
 * Get some locale information.
 */

#include <string>
#include <cstddef>
//#include <chrono>
//#include <cyng.h>
//#include <boost/version.hpp>
//#include <boost/asio/version.hpp>
//#include <boost/beast/version.hpp>

namespace cyng {
	namespace sys
	{
		std::string get_system_locale();
		//std::string get_system_country();
	}
}
#endif

