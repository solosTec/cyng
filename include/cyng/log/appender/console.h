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
#ifndef CYNG_LOG_APPENDER_CONSOLE_H
#define CYNG_LOG_APPENDER_CONSOLE_H

#include <cyng/log/appender.h>

#include <boost/predef.h>	//	requires Boost 1.55

#if BOOST_OS_WINDOWS
#include <process.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#include <Windows.h>
#endif
#endif

namespace cyng {
	namespace logging {	

		class console
		{
		public:
			console();
			~console();

			void write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg);

		private:
			bool init();
			void cls();

		private:
#if BOOST_OS_WINDOWS
			HANDLE h_out_;
			CONSOLE_SCREEN_BUFFER_INFO csbi_info_;	//!<	save current text colors
#endif
			bool terminal_seq_enabled_;
		};


	}
}

#endif

