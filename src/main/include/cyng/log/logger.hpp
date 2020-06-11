/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_LOGGER_HPP
#define CYNG_LOG_LOGGER_HPP

#include <cyng/log/severity.h>
#include <cyng/chrono.h>
#include <cyng/compatibility/file_system.hpp>
#include <boost/predef.h>	//	requires Boost 1.55
#include <cyng/compatibility/io_service.h>
#include <stdexcept>
#include <iomanip>
#include <fstream>
#if BOOST_OS_WINDOWS
#include <process.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#endif

#if BOOST_OS_LINUX
#include <syslog.h>
#endif

namespace cyng 
{
	namespace logging 
	{
		struct console_out{};
		struct file_out{};
#if BOOST_OS_LINUX
		struct syslog_out{};
#endif

		/**
		 * Define minimal interface for logger class
		 */
		template <typename R>
		class log_base
		{
		protected:
			using record_type = R;
			using record_ptr = std::shared_ptr< record_type >;
			
		public:
			virtual bool push(record_ptr ptr) = 0;
			virtual void set_severity(severity) = 0;
		};
		
		/**
		 * A logger is parameterized by it's record type.
		 * 
		 * @tparam R record type
		 * @tparam S output system
		 */
		template <typename R, typename S>
		class logger : public log_base< R >
		{
			using stream_type = S;
			using this_type = logger< typename log_base< R >::record_type,  stream_type>;
			
		public:
			/**
			 * append a new log entry synchronously 
			 */
			virtual bool push(typename log_base< R >::record_ptr) override
			{
				return false;
			}

			virtual void set_severity(severity) override
			{}
		};
		
		/**
		 * A filestream logger
		 * 
		 * @tparam R record type
		 */
		template <typename R>
		class logger <R, file_out> : public log_base< R >
		{
			using stream_type = file_out;
			using this_type = logger< typename log_base< R >::record_type,  stream_type>;
			using logstream = std::ofstream;
			
		public:
			logger(io_service_t& ioc
				, filesystem::path const& p
				, severity threshold = severity::LEVEL_TRACE
				, std::size_t rotation_size = 32UL * 1024UL * 1024UL	//	32 MB
			)
			: dispatcher_(ioc)
				, path_(p)
				, logstream_(path_.string(), std::fstream::app)
				, threshold_(threshold)
				, rotation_size_(rotation_size)
				, uncommitted_(0)
			{}
			
			/**
			 * append a new log entry asynchronously 
			 */
			virtual bool push(typename log_base< R >::record_ptr ptr) override
			{
#if (BOOST_ASIO_VERSION < 101200)
				BOOST_ASSERT_MSG(!dispatcher_.get_io_service().stopped(), "no io service available");
#elif (BOOST_ASIO_VERSION < 101202)
				BOOST_ASSERT_MSG(!dispatcher_.get_io_context().stopped(), "no io service available");
#else
				BOOST_ASSERT_MSG(!dispatcher_.context().stopped(), "no io service available");
#endif
					
				
				
				if (!ptr->is_matching_severity(threshold_))	return false;
				
				dispatcher_.dispatch([this, ptr](){
					try 
					{
						++uncommitted_;
						this->write(ptr);
						if (uncommitted_ > 64)
						{
							//
							//	reset uncommited counter
							//
							uncommitted_ = 0;
							
							//
							//	write cache to disc
							//
							this->logstream_.flush();
							
							//
							//	test file size
							//
							test_file_size();
						}
					}
					catch (std::runtime_error const& ex) 	
					{
						std::cerr
						<< ex.what()
						<< std::endl
						;
					}
				});
				
				return true;
			}
			
			virtual void set_severity(severity s) override
			{
				threshold_ = s;
			}

		private:
			void write(typename log_base< R >::record_ptr ptr)
			{
				//
				//	To write simple '\n' instead of std::endl
				//	speeds up writing by factor > 20
				//
				logstream_
					<< *ptr
					<< '\n'
					;
				
			}

			void test_file_size()
			{
				const auto fs = filesystem::file_size(path_);
				if (fs > rotation_size_)	
				{	//	32 MB

					//
					//	close file
					//
					logstream_.close();

					//
					//	create backup file
					//
					create_backup_file();

					//
					//	reopen file
					//
					logstream_.open(path_.string());
				}
			}
			
			void create_backup_file()
			{
				std::pair<std::time_t, double> r = chrono::to_dbl_time_point(std::chrono::system_clock::now());
				std::tm tm = cyng::chrono::convert_utc(r.first);

				//	build a filename for backup file
				std::stringstream ss;
				ss
						<< chrono::year(tm)
						<< 'T'
						<< std::setw(3)
						<< std::setfill('0')
						<< chrono::day_of_year(tm)
						<< '_'
						<< chrono::time_of_day(tm)	// in seconds
						;

				auto const tag = ss.str();
				auto const backup = path_.parent_path() / ((path_.stem().string() + "_backup_" + tag) + path_.extension().string());

				filesystem::rename(path_, backup);
			}
			
		private:
			dispatcher_t dispatcher_;
			const filesystem::path	path_;
			logstream logstream_;
			severity threshold_;
			const std::size_t	rotation_size_;
			volatile std::size_t uncommitted_;
		};

		/**
		 * A console logger
		 * 
		 * @tparam R record type
		 */
		template <typename R>
		class logger <R, console_out> : public log_base< R >
		{
			using stream_type = console_out;
			using this_type = logger< typename log_base< R >::record_type,  stream_type>;
			
		public:
			logger(io_service_t& ioc
				, std::string const& ident
				, severity threshold = severity::LEVEL_TRACE
			)
			: dispatcher_(ioc)
			, threshold_(threshold)
#if BOOST_OS_WINDOWS
			, h_out_(NULL)
			, csbi_info_()
			, terminal_seq_enabled_(false)
#else
			, terminal_seq_enabled_(true)
#endif
			{
				std::stringstream ss;
				ss
				<< ident 
				<< ':'
#if BOOST_OS_WINDOWS
				<< ::_getpid()
#else
				<< ::getpid()	//	POSIX
#endif
				;
				
				init(ss.str());
			}
			
			virtual ~logger()
			{
				if (terminal_seq_enabled_) {
					//	reset terminal
					std::cerr
						<< "\033[0m"	//	reset
						<< std::endl
						;

				}
			}

			/**
			 * append a new log entry synchronously 
			 */
			virtual bool push(typename log_base< R >::record_ptr ptr) override
			{
 				
#if (BOOST_ASIO_VERSION < 101200)
				BOOST_ASSERT_MSG(!dispatcher_.get_io_service().stopped(), "no io service available");
#elif (BOOST_ASIO_VERSION < 101202)
				BOOST_ASSERT_MSG(!dispatcher_.get_io_context().stopped(), "no io service available");
#else
				BOOST_ASSERT_MSG(!dispatcher_.context().stopped(), "no io service available");
#endif
					
				
				if (!ptr->is_matching_severity(threshold_))	return false;
				
				dispatcher_.dispatch([this, ptr](){
					if (ptr->is_matching_severity(severity::LEVEL_FATAL))
					{
						print_fatal_msg(ptr);
					}
					else if (ptr->is_matching_severity(severity::LEVEL_ERROR))
					{
						print_error_msg(ptr);
					}
					else if (ptr->is_matching_severity(severity::LEVEL_WARNING))
					{
						print_warning_msg(ptr);
					}
					else if (ptr->is_matching_severity(severity::LEVEL_INFO))
					{
						print_info_msg(ptr);
					}
					else 
					{
						if (std::cout.bad())
						{
							std::cout.clear();
						}
						std::cout
							<< *ptr
							<< std::endl
							;
					}
				});
				
				return true;
			}

			virtual void set_severity(severity s) override
			{
				threshold_ = s;
			}

		private:
			void print_fatal_msg(typename log_base< R >::record_ptr ptr)
			{
				if (std::cerr.bad())
				{
					std::cerr.clear();
				}
				if (terminal_seq_enabled_)
				{
					std::cerr
						<< "\033[1m\033[31m"	//	bold red
						<< *ptr
						<< "\033[0m"	//	reset
						<< std::endl
						;
				}
				else
				{
					std::cerr
						<< *ptr
						<< std::endl
						;
				}
			}

			void print_error_msg(typename log_base< R >::record_ptr ptr)
			{
				if (std::cerr.bad())
				{
					std::cerr.clear();
				}
				if (terminal_seq_enabled_)
				{
					std::cerr
						<< "\033[31m"	//	red
						<< *ptr
						<< "\033[0m"	//	reset
						<< std::endl
						;
				}
				else
				{
					std::cerr
						<< *ptr
						<< std::endl
						;
				}
			}
			
			void print_warning_msg(typename log_base< R >::record_ptr ptr)
			{
				if (std::cerr.bad())
				{
					std::cerr.clear();
				}
				if (terminal_seq_enabled_)
				{
					std::cerr
						<< "\033[33m"	//	yellow
						<< *ptr
						<< "\033[0m"	//	reset
						<< std::endl
						;
				}
				else
				{
					std::cerr
						<< *ptr
						<< std::endl
						;
				}
			}
			
			void print_info_msg(typename log_base< R >::record_ptr ptr)
			{
				if (std::cerr.bad())
				{
					std::cerr.clear();
				}
				if (terminal_seq_enabled_)
				{
					std::cerr
						<< "\033[32m"	//	green
						<< *ptr
						<< "\033[0m"	//	reset
						<< std::endl
						;
				}
				else
				{
					std::cerr
						<< *ptr
						<< std::endl
						;
				}
			}
			
			bool init(std::string const& title)
			{
#if BOOST_OS_WINDOWS
				
				//
				//	set console title
				//
				::SetConsoleTitle(title.c_str());
				
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
				std::cout
					<< "\033]2;"
					<< title
					<< "\07"
					;
#endif				
				
				cls();
				
				return true;
			}
			
			void cls()
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
				//	ToDo: implement POSIX version
#endif
			}
			
		private:
			dispatcher_t dispatcher_;
			severity threshold_;
#if BOOST_OS_WINDOWS
			HANDLE h_out_;
			CONSOLE_SCREEN_BUFFER_INFO csbi_info_;	//!<	save current text colors
#endif
			bool terminal_seq_enabled_;			
		};
		
#if BOOST_OS_LINUX
		/**
		 * A logger is parameterized by it's record type.
		 * 
		 * @tparam R record type
		 * @tparam S output system
		 */
		template <typename R>
		class logger<R, syslog_out> : public log_base< R >
		{
			using stream_type = syslog_out;
			using this_type = logger< typename log_base< R >::record_type, stream_type>;
			
		public:
			logger(const char* ident
			, bool console = false
			, severity threshold = severity::LEVEL_TRACE)
			: threshold_(threshold)
			{
				init(ident, console);
			}
			
			virtual ~logger()
			{
				closelog();
			}
			
			/**
			 * append a new log entry synchronously 
			 */
			virtual bool push(typename log_base< R >::record_ptr ptr) override
			{
				if (!ptr->is_matching_severity(threshold_))	return false;
				
				std::stringstream ss;
				ss <<  *ptr;
				const std::string msg = ss.str();
				
				if (ptr->is_matching_severity(severity::LEVEL_ERROR))
				{
					syslog (LOG_ERR, "%s", msg.c_str());
				}
				else if (ptr->is_matching_severity(severity::LEVEL_WARNING))
				{
					syslog (LOG_WARNING, "%s", msg.c_str());
				}
				else if (ptr->is_matching_severity(severity::LEVEL_INFO))
				{
					syslog (LOG_INFO, "%s", msg.c_str());
				}
				else 
				{
					syslog (LOG_DEBUG, "%s", msg.c_str());
// 					std::cout
// 						<< *ptr
// 						<< std::endl
// 						;
				}
				return true;
			}

			virtual void set_severity(severity s) override
			{
				threshold_ = s;
			}

		private:
			void init(const char* ident, bool console)
			{
// 				std::cout << "init(syslog)" << std::endl;
				//	LOG_EMERG
				//	LOG_ALERT
				//	LOG_CRIT
				//	LOG_ERR
				//	LOG_WARNING
				//	LOG_NOTICE
				//	LOG_INFO
				//	LOG_DEBUG
				setlogmask(LOG_UPTO (LOG_DEBUG));
				
				//	LOG_SYSLOG
				//	LOG_LOCAL0-7
				if (console)
				{
					openlog (ident, LOG_CONS | LOG_PID | LOG_NDELAY | LOG_PERROR, LOG_USER);
				}
				else 
				{
					openlog (ident, LOG_PID | LOG_NDELAY, LOG_USER);
				}
				syslog (LOG_ERR, "started by user %d", getuid ());
			}
			
		private:
			severity threshold_;
		};
#endif
		
	}	
}

#endif	//	CYNG_LOG_LOGGER_HPP


