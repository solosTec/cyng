#include <cyng/log/appender/journald.h>

namespace cyng {
	namespace logging {

		journald::journald(std::string ident, bool console)
		{
			//	LOG_EMERG, LOG_ALERT, LOG_CRIT, LOG_ERR, LOG_WARNING, LOG_NOTICE, LOG_INFO, LOG_DEBUG
		}
		journald::~journald()
		{	}

		void journald::write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg) {
			switch(lev)	{
			case severity::LEVEL_TRACE:
                sd_journal_print( LOG_NOTICE, "%s", msg.c_str() );
				break;
			case severity::LEVEL_INFO:
                sd_journal_print( LOG_INFO, "%s", msg.c_str() );
				break;
			case severity::LEVEL_WARNING:
                sd_journal_print( LOG_WARNING, "%s", msg.c_str() );
				break;
			case severity::LEVEL_ERROR:
                sd_journal_print( LOG_ERR, "%s", msg.c_str() );
				break;
			case severity::LEVEL_FATAL:
                sd_journal_print( LOG_CRIT, "%s", msg.c_str() );
				break;
			case severity::LEVEL_DEBUG:
			default:
                sd_journal_print( LOG_DEBUG, "%s", msg.c_str() );
				break;
			}
		}
	}
}

