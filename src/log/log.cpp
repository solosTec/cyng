/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/log.h>
#include <cyng/io/ostream.h>
#include <cyng/log/appender/console.h>
#include <cyng/log/appender/rolling_file.h>

#ifdef APPENDER_EVENT_LOG
#include <cyng/log/appender/event_log.h>
#endif

#ifdef APPENDER_SYS_LOG
#include <cyng/log/appender/sys_log.h>
#endif

namespace cyng {

	log::log()
		: sigs_{
		[&](std::chrono::system_clock::time_point ts, severity lev, std::uint32_t tid, std::string msg) {	this->write(ts, lev, tid, msg); },
		std::bind(&log::start_console, this),
		std::bind(&log::start_file, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&log::start_sys_log, this, std::placeholders::_1, std::placeholders::_2),
		std::bind(&log::start_event_log, this),
		[&](severity s) {	this->set_level(s); },
		std::bind(&log::stop, this, std::placeholders::_1)
	}
		, level_(severity::LEVEL_TRACE)
		, con_()
		, rfile_()
//#ifdef APPENDER_SYS_LOG
//		, sys_()
//#endif
//#ifdef APPENDER_EVENT_LOG
//		, event_()
//#endif
	{}

	log::~log()
	{}

	void log::stop(eod) {

		//
		//	write to console
		//
		if (con_) {
			con_->write(std::chrono::system_clock::now(), severity::LEVEL_INFO, 0, "stop console logger");
			con_.reset();
		}

		//
		//	write to rolling file
		//
		if (rfile_) {
			rfile_->write(std::chrono::system_clock::now(), severity::LEVEL_INFO, 0, "stop file logger");
			rfile_.reset();
		}

		//
		//	write to event log
		//
#ifdef APPENDER_EVENT_LOG
		//if (event_) {
		//	event_->write(std::chrono::system_clock::now(), severity::LEVEL_INFO, 0, "stop event logger");
		//	event_.reset();
		//}
#endif

		//
		//	write to sys log
		//
#ifdef APPENDER_SYS_LOG
		//if (sys_) {
		//	sys_->write(std::chrono::system_clock::now(), severity::LEVEL_INFO, 0, "stop sys logger");
		//	sys_.reset();
		//}
#endif

	}

	void log::write(std::chrono::system_clock::time_point ts, severity lev, std::uint32_t tip, std::string msg) {
		if (lev >= level_) {

			//
			//	select appenders
			//

			//
			//	write to console
			//
			if (con_)	con_->write(ts, lev, tip, msg);

			//
			//	write to rolling file
			//
			if (rfile_)	rfile_->write(ts, lev, tip, msg);

			//
			//	write to event log
			//
#ifdef APPENDER_EVENT_LOG
			//if (event_)	event_->write(ts, lev, tip, msg);
#endif

			//
			//	write to sys log
			//
#ifdef APPENDER_SYS_LOG
			if (sys_)	sys_->write(ts, lev, tip, msg);
#endif

		}
	}

	void log::start_console() {
		con_.reset(new logging::console());
	}
	void log::start_file(std::filesystem::path path, std::uint64_t size) {
		BOOST_ASSERT(size != 0);
		BOOST_ASSERT(!path.empty());
		if (con_) con_->write(std::chrono::system_clock::now(), severity::LEVEL_INFO, 0, "start file logger");
		rfile_.reset(new logging::rolling_file(path, size));
	}
	void log::start_sys_log(std::string ident, bool console) {

#ifdef APPENDER_SYS_LOG
		sys_.reset(new logging::sys_log(ident, console));
#endif

	}
	void log::start_event_log() {
#ifdef APPENDER_EVENT_LOG
		//event_.reset(new logging::eventlog());
#endif
	}

	void log::set_level(severity lev) {
		level_ = lev;
	}
}

