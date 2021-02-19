/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LOG_RECORD_H
#define CYNG_LOG_RECORD_H

#include <cyng/obj/intrinsics/severity.h>
#include <cyng/io/ostream.h>

#include <string>
#include <chrono>
#include <sstream>
#include <thread>
#include <memory>

#include<boost/core/ignore_unused.hpp>

namespace cyng {

	namespace logging	{	//	don't collide with store records
		struct record : public std::enable_shared_from_this<record> {

			std::chrono::system_clock::time_point const tp_;
			std::thread::id	const tid_;
			severity const level_;
			std::stringstream stream_;

			record(severity lev, std::string msg);

			/**
			 * The overloaded stream operator simplify the use of the logger class
			 *
			 * @code
			 * auto msg = record::factory(severity::LEVEL_INFO);
			 * (*msg) << "more" << " data";
			 * @endcode
			 */
			template < typename T >
			friend record& operator<< (record& rec, T const& v)
			{
				using cyng::operator<<;

				rec.stream_ << v;
				return rec;
			}

			/**
			 * Another overload for std stream manipulators such as std::endl, std::dec, ...
			 * @note Only works for std::basic_ostream at the moment
			 *
			 * @code
			 * auto msg = record::factory(severity::LEVEL_INFO);
			 * (*msg) << std::hex << 2134 << std::endl;
			 * @endcode
			 */
			friend record& operator<< (record& rec, std::ostream& (*manip)(std::ostream&))
			{
				rec.stream_ << manip;
				return rec;
			}

			static std::shared_ptr<record> create(severity level, std::string msg = "")
			{
				return std::make_shared<record>(level, msg);
			}

			std::uint32_t tid() const;
			std::string msg() const;

		};

		using entry = std::shared_ptr<record>;

		template < typename T >
		entry operator<< (entry rec, T const& v)
		{
			*rec << v;
			return rec;
		}

	}



#ifdef _DEBUG
	std::string to_str(logging::record const&);
#endif
}

//
//	Some shortcuts
//	Note that the (only) variable __msg could hide the visibility
//	of a an outside variable with the same name.
//
#define CYNG_LOG(l,s,m)	\
	{	\
		using cyng::operator<<; \
		auto __msg = cyng::logging::record::create(s);	\
		__msg << m;	\
		l.push(*__msg);	\
	}

#define CYNG_LOG_TRACE(l,m)	\
	CYNG_LOG(l,cyng::severity::LEVEL_TRACE,m)

#ifdef _DEBUG
#define CYNG_LOG_DEBUG(l,m)	\
	CYNG_LOG(l,cyng::severity::LEVEL_DEBUG,m)
#else
#define CYNG_LOG_DEBUG(l,m)	\
	boost::ignore_unused(l);
#endif

#define CYNG_LOG_INFO(l,m)	\
	CYNG_LOG(l,cyng::severity::LEVEL_INFO,m)

#define CYNG_LOG_WARNING(l,m)	\
	CYNG_LOG(l,cyng::severity::LEVEL_WARNING,m)

#define CYNG_LOG_ERROR(l,m)	\
	CYNG_LOG(l,cyng::severity::LEVEL_ERROR,m)

#define CYNG_LOG_FATAL(l,m)	\
	CYNG_LOG(l,cyng::severity::LEVEL_FATAL,m)

#endif
