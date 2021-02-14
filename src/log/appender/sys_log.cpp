/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/appender/sys_log.h>

namespace cyng {
	namespace logging {

		syslog::syslog()
		{}
		syslog::~syslog()
		{}

		void syslog::write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg) {

		}
	}
}

