/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/appender/console.h>
#include <cyng/log/appender.h>

#include <iostream>

namespace cyng {
	namespace logging {

		console::console()
#if BOOST_OS_WINDOWS
			: terminal_seq_enabled_(false)
			, h_out_(NULL)
			, csbi_info_()
#else
			: terminal_seq_enabled_(true)
#endif
		{
			init();
		}

		console::~console() {
			if (terminal_seq_enabled_) {
				//	reset terminal
				std::cerr
					<< "\033[0m"	//	reset
					<< std::endl
					;

			}
		}

		void console::write(std::chrono::system_clock::time_point ts, severity lev, std::uint64_t tip, std::string const msg) {

			if (terminal_seq_enabled_) {
				switch (lev) {
				case severity::LEVEL_TRACE:
				case severity::LEVEL_DEBUG:
					//	no color
					std::cerr
						<< format(ts, lev, tip, msg)
						<< std::endl
						;
					break;

				case severity::LEVEL_INFO:
					std::cerr
						<< "\033[32m"	//	green
						<< format(ts, lev, tip, msg)
						<< "\033[0m"	//	reset
						<< std::endl
						;
					break;
				case severity::LEVEL_WARNING:
					std::cerr
						<< "\033[33m"	//	yellow
						<< format(ts, lev, tip, msg)
						<< "\033[0m"	//	reset
						<< std::endl
						;
					break;
				case severity::LEVEL_ERROR:
					std::cerr
						<< "\033[31m"	//	red
						<< format(ts, lev, tip, msg)
						<< "\033[0m"	//	reset
						<< std::endl
						;
					break;
				case severity::LEVEL_FATAL:
					std::cerr
						<< "\033[1m\033[31m"	//	bold red
						<< format(ts, lev, tip, msg)
						<< "\033[0m"	//	reset
						<< std::endl
						;
					break;
				default:
					break;
				}
			}
			else {
				std::cerr
					<< format(ts, lev, tip, msg)
					<< std::endl
					;
			}
		}


		bool console::init() {
#if BOOST_OS_WINDOWS
			//
			//	set console title
			//
			//::SetConsoleTitle(title.c_str());

			h_out_ = ::GetStdHandle(STD_OUTPUT_HANDLE);
			if (h_out_ == INVALID_HANDLE_VALUE)
			{
				std::cerr
					<< "***error: cannot get std handle for console output"
					<< std::endl
					;
				return false;
			}

			DWORD dwMode = 0;
			if (!::GetConsoleMode(h_out_, &dwMode))
			{
				std::cerr
					<< "***error: cannot get console mode"
					<< std::endl
					;
				return false;
			}

			if (!::GetConsoleScreenBufferInfo(h_out_, &csbi_info_))
			{
				std::cerr
					<< "***error: cannot get console screen buffer info"
					<< std::endl
					;
			}

			//	ENABLE_VIRTUAL_TERMINAL_PROCESSING == 0x0004
			if (!::SetConsoleMode(h_out_, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING))
			{
				std::cerr
					<< "***warning: cannot enable ENABLE_VIRTUAL_TERMINAL_PROCESSING"
					<< std::endl
					;

				//
				// reset previos mode
				//
				::SetConsoleMode(h_out_, dwMode);
				return false;
			}

			//
			//	enable Console Virtual Terminal Sequences 
			//
			terminal_seq_enabled_ = true;

#endif
#if BOOST_OS_LINUX
			//	Set Window Title 
			//	ESC ] 2; <string> BEL
			//std::cout
			//	<< "\033]2;"
			//	<< title
			//	<< "\07"
			//	;
#endif		
			//
			//	clear
			//
			cls();

			return true;
		}

		void console::cls()
		{
#if BOOST_OS_WINDOWS
			COORD coord_screen{ 0, 0 };    // home for the cursor 
			DWORD chars_written = 0;
			DWORD con_size = csbi_info_.dwSize.X * csbi_info_.dwSize.Y;

			if (!::FillConsoleOutputCharacter(h_out_,        // Handle to console screen buffer 
				0x20,		// UTF-8 SPACE
				//'.',		// Character to write to the buffer
				con_size,			// Number of cells to write 
				coord_screen,		// Coordinates of first cell 
				&chars_written))	// Receive number of characters written
			{
				return;
			}

			if (!::FillConsoleOutputAttribute(h_out_,         // Handle to console screen buffer 
				csbi_info_.wAttributes,	// Character attributes to use
				con_size,			// Number of cells to set attribute 
				coord_screen,      // Coordinates of first cell 
				&chars_written))	// Receive number of characters written
			{
				return;
			}

			//	reset cursor
			::SetConsoleCursorPosition(h_out_, coord_screen);
#else
			//	POSIX version
			std::cout << "\033[2J\033[1;1H";
#endif
		}

	}
}

