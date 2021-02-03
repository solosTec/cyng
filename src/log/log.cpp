/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/log.h>
#include <cyng/io/ostream.h>

namespace cyng {

	log::log()
		: sigs_{
		[&](std::chrono::system_clock::time_point ts, severity lev, std::uint32_t tid, std::string msg) {	this->write(ts, lev, tid, msg); },
		std::bind(&log::flush, this),
		[&](severity s) {	this->set_level(s); },
		std::bind(&log::stop, this, std::placeholders::_1)
	}
		, level_(severity::LEVEL_TRACE)
		, con_()
		, rfile_(std::filesystem::temp_directory_path() / "docc.log", 32UL * 1024UL * 1024UL)	//	32MB
	{}

	log::~log()
	{}

	void log::stop(eod) {

	}

	//void log::write(object const& obj) {

	//}

	void log::write(std::chrono::system_clock::time_point ts, severity lev, std::uint32_t tip, std::string msg) {
		if (lev >= level_) {

			//
			//	select appenders
			//

			//
			//	write to console
			//
			con_.write(ts, lev, tip, msg);

			//
			//	write to rolling file
			//
			rfile_.write(ts, lev, tip, msg);
		}
	}

	void log::flush() {

	}

	void log::set_level(severity lev) {
		level_ = lev;
	}
}

