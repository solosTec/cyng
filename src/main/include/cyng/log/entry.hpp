/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_ENTRY_HPP
#define CYNG_LOG_ENTRY_HPP

#include <cyng/log/severity.h>
#include <cyng/io/io_chrono.hpp>
#include <chrono>
#include <sstream>
#include <thread>
#include <boost/predef.h>

namespace cyng 
{
	namespace logging 
	{

		struct TAG_ALL {};
		struct TAG_NO_THREAD_ID {};
		

		/**
		 * Most complete logging entry
		 */
		template < typename TAG = TAG_ALL >
		struct entry 
		: std::enable_shared_from_this<entry<TAG> >
		{
			const logging::severity	level_;
			std::stringstream stream_;
			const std::chrono::system_clock::time_point	tp_;
			const std::thread::id	thread_id_;
			
			entry(severity level, std::string const& msg)
			: level_(level)
			, stream_(msg)
			, tp_(std::chrono::system_clock::now())
			, thread_id_(std::this_thread::get_id())
			{}
			
			bool is_matching_severity(severity level)
			{
				return level_ >= level;
			}
			
			friend std::ostream& operator<<(std::ostream& os, entry const& rec)
			{
				//	otherwise stream content is moved and lost
				const std::string msg = rec.stream_.str();
				
				//
				//	want to use stream operator from namespace cyng
				// 	cyng::operator<<(os, rec.tp_);
				//
				using cyng::operator<<;	
				
				os 
					<< '['
					<< rec.tp_
					<< ']'
					<< ' '
                    << to_string(rec.level_)
					<< ' '
#if BOOST_OS_WINDOWS
					<< std::dec
					<< std::setfill(' ')
					<< std::setw(5)
#endif
					<< rec.thread_id_
					<< " -- "
					<< msg
					;
				return os;
			}
			
			/**
			 * The overloaded stream operator simplify the use of the logger class
			 * 
			 * @code
			 * auto msg = cyng::entry::factory(logging::severity::LEVEL_INFO);
			 * (*msg) << "more" << " data";
			 * @endcode
			 */
			template < typename T >
			friend entry& operator<< (entry& ls, T const& v)	
			{
				ls.stream_ << v;
				return ls;
			}
			
			/**
			 * Another overload for std stream manipulators such as std::endl, std::dec, ...
			 * @note Only works for std::basic_ostream at the moment
			 * 
			 * @code
			 * auto msg = cyng::entry::factory(logging::severity::LEVEL_INFO);
			 * (*msg) << std::hex << 2134 << std::endl;
			 * @endcode
			 */
			friend entry& operator<< (entry& ls, std::ostream& (*manip)(std::ostream&))
			{
				ls.stream_ << manip;
				return ls;
			}
			
			static std::shared_ptr<entry> create(severity level)
			{
				return std::make_shared<entry>(level, "");
			}
			
		};
				
		/**
		 * Logging without thread id.
		 */
		template <>
		struct entry< TAG_NO_THREAD_ID >
		: std::enable_shared_from_this<entry<TAG_NO_THREAD_ID> >
		{
			const logging::severity	level_;
			std::stringstream stream_;
			const std::chrono::system_clock::time_point	tp_;
			
			entry(severity level, std::string const& msg)
			: level_(level)
			, stream_(msg)
			, tp_(std::chrono::system_clock::now())
			{}

			bool is_matching_severity(severity level)
			{
				return level_ >= level;
			}

			friend std::ostream& operator<<(std::ostream& os, entry const& rec)
			{
				//
				//	want to use stream operator from namespace cyng
				// 	cyng::operator<<(os, rec.tp_);
				//
				using cyng::operator<<;	
				
				os 
				<< '['
				<< rec.tp_
				<< ']'
				<< ' '
                << to_string(rec.level_)
				<< " -- "
// 				<< rec.stream_
				;
				return os;
			}
						
		};
		
	}	
}

#endif	//	CYNG_LOG_ENTRY_HPP


