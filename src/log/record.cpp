/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/record.h>

#include <functional>
#include <boost/predef.h>

namespace docscript {
	namespace logging {

		record::record(severity lev, std::string msg)
			: tp_(std::chrono::system_clock::now())
			, tid_(std::this_thread::get_id())
			, level_(lev)
			, stream_()
		{
			stream_ << msg;
		}

		std::uint32_t record::tid() const {
			
			return static_cast<std::uint32_t>(std::hash<std::thread::id>()(tid_) & std::numeric_limits<std::uint32_t>::max());

		}

		std::string record::msg() const {
			return stream_.str();
		}


	}

#ifdef _DEBUG

	std::string to_str(logging::record const& rec) {

		std::stringstream ss;
		ss
			<< '['
			<< rec.tp_
			<< ']'
			<< ' '
			<< rec.level_
			<< ' '
#if BOOST_OS_WINDOWS
			<< std::dec
			<< std::setfill(' ')
			<< std::setw(5)
#endif
			<< rec.tid_
			<< " -- "
			<< rec.stream_.rdbuf()	//	stream is moved
			;
		;

		return ss.str();
	}
#endif

}

