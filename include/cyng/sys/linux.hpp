/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_SYS_LINUX_HPP
#define CYNG_SYS_LINUX_HPP

/** @file linux.hpp
 * Some linux specific tools to get system information
 */

#include <string>
#include <cstddef>
#include <fstream>
#include <boost/predef.h>

namespace cyng {
	namespace sys
	{
#if defined(BOOST_OS_LINUX_AVAILABLE)

		/**
		 * Extract values from a system file like "/proc/uptime.
		 * You can read other files too.
		 */
		template <typename ...Args>
		auto read_virtual_file(std::string name, Args& ...args)	-> std::tuple<Args...> {
			std::ifstream ifs(name);
			((ifs >> args), ...);
			return std::tuple<Args...>(std::forward<Args>(args)...);
		}

#endif
	}
}
#endif

