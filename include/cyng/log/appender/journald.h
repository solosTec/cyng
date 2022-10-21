/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_LOG_APPENDER_JOURNALD_H
#define CYNG_LOG_APPENDER_JOURNALD_H

#ifdef APPENDER_JOURNALD

#include <cyng/log/appender.h>
#include <systemd/sd-journal.h>	//	system lib

namespace cyng {
	namespace logging {
		class journald
		{
		public:
			journald(std::string ident, bool console);
			~journald();

			void write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg);
		};
	}
}

#endif
#endif

