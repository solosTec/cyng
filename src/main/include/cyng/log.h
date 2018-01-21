/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_H
#define CYNG_LOG_H

#include <cyng/log/severity.h>
#include <cyng/log/entry.hpp>
#include <cyng/log/logger.hpp>

namespace cyng 
{
	namespace logging 
	{
		using entry_t = entry<TAG_ALL>;
		using file_logger_t = logger <entry_t, file_out>;
		using console_logger_t = logger <entry_t, console_out>;
#if BOOST_OS_LINUX
		using sys_logger_t = logger <entry_t, syslog_out>;
#endif
		/**
		 * Define the essential logger interface 
		 */
		using logger_interface = log_base<entry_t>;
		using log_ptr = std::shared_ptr<logger_interface>;
		
		inline log_ptr make_file_logger(boost::asio::io_service& ios
			, boost::filesystem::path const& p
			, severity threshold = severity::LEVEL_TRACE
			, std::size_t rotation_size = 32UL * 1024UL * 1024UL)
		{
			return std::static_pointer_cast<logger_interface>(std::make_shared<file_logger_t>(ios, p, threshold, rotation_size));
		}
		
		inline log_ptr make_console_logger(boost::asio::io_service& ios
			, std::string const& ident
			, severity threshold = severity::LEVEL_TRACE)
		{
			return std::static_pointer_cast<logger_interface>(std::make_shared<console_logger_t>(ios, ident, threshold));
		}
		
#if BOOST_OS_LINUX
		inline log_ptr make_sys_logger(const char* ident
			, bool console = false
			, severity threshold = severity::LEVEL_TRACE)
		{
			return std::static_pointer_cast<logger_interface>(std::make_shared<sys_logger_t>(ident, console, threshold));
		}
#endif
	}
}

//
//	Some shortcuts
//	Note that the (only) variable __msg could hide the visibility
//	of a an outside variable with the same name.
//
#define CYNG_LOG(l,s,m)	\
	{	\
		using cyng::operator<<; \
		auto __msg = cyng::logging::entry_t::create(s);	\
		(*__msg) << m;	\
		l->push(__msg);	\
	}

#define CYNG_LOG_TRACE(l,m)	\
	CYNG_LOG(l,cyng::logging::severity::LEVEL_TRACE,m)

#define CYNG_LOG_DEBUG(l,m)	\
	CYNG_LOG(l,cyng::logging::severity::LEVEL_DEBUG,m)

#define CYNG_LOG_INFO(l,m)	\
	CYNG_LOG(l,cyng::logging::severity::LEVEL_INFO,m)

#define CYNG_LOG_WARNING(l,m)	\
	CYNG_LOG(l,cyng::logging::severity::LEVEL_WARNING,m)
	
#define CYNG_LOG_ERROR(l,m)	\
	CYNG_LOG(l,cyng::logging::severity::LEVEL_ERROR,m)

#define CYNG_LOG_FATAL(l,m)	\
	CYNG_LOG(l,cyng::logging::severity::LEVEL_FATAL,m)


#endif	//	CYNG_LOG_H

