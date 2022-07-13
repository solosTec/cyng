/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/appender/sys_log.h>

namespace cyng {
	namespace logging {

		sys_log::sys_log(std::string ident, bool console)
		{
			//	LOG_EMERG
			//	LOG_ALERT
			//	LOG_CRIT
			//	LOG_ERR
			//	LOG_WARNING
			//	LOG_NOTICE
			//	LOG_INFO
			//	LOG_DEBUG
			setlogmask(LOG_UPTO (LOG_DEBUG));
			if (console) {
				openlog (ident.c_str(), LOG_CONS | LOG_PID | LOG_NDELAY | LOG_PERROR, LOG_USER);
			}
			else {
				openlog (ident.c_str(), LOG_PID | LOG_NDELAY, LOG_USER);
			}
		}
		sys_log::~sys_log()
		{
			closelog();
		}

		void sys_log::write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg) {
			switch(lev)	{
			case severity::LEVEL_TRACE:
				syslog (LOG_NOTICE, "%s", msg.c_str());
				break;
			case severity::LEVEL_INFO:
				syslog (LOG_INFO, "%s", msg.c_str());
				break;
			case severity::LEVEL_WARNING:
				syslog (LOG_WARNING, "%s", msg.c_str());
				break;
			case severity::LEVEL_ERROR:
				syslog (LOG_ERR, "%s", msg.c_str());
				break;
			case severity::LEVEL_FATAL:
				syslog (LOG_CRIT, "%s", msg.c_str());
				break;
			case severity::LEVEL_DEBUG:
			default:
				syslog (LOG_DEBUG, "%s", msg.c_str());
				break;
			}
		}
	}
}

