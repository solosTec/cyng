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
#ifndef CYNG_LOG_APPENDER_SYS_LOG_H
#define CYNG_LOG_APPENDER_SYS_LOG_H

#ifdef APPENDER_SYS_LOG

#include <cyng/log/appender.h>

namespace cyng {
	namespace logging {
		class syslog
		{
		public:
			syslog();
			~syslog();

			void write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg);
		};
	}
}

#endif
#endif

