/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
 /*
  * The MIT License (MIT)
  *
  * Copyright (c) 2021 Sylko Olzscher
  *
  */
#ifndef CYNG_LOG_APPENDER_H
#define CYNG_LOG_APPENDER_H

#include <cyng/obj/intrinsics/severity.h>
#include <chrono>
#include <string>

namespace cyng {
	namespace logging {

		std::string format(std::chrono::system_clock::time_point ts, severity lev, std::uint32_t tip, std::string const msg);
	}
}

#endif

