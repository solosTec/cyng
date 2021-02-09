/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/log/appender.h>
#include <cyng/io/ostream.h>

#include <sstream>

namespace cyng {
	namespace logging {

		std::string format(std::chrono::system_clock::time_point tp, severity lev, std::uint32_t tip, std::string const msg) {
			std::stringstream ss;
			ss
				<< '['
				<< tp
				<< ']'
				<< ' '
				<< lev
				<< ' '
#if BOOST_OS_WINDOWS
				<< std::dec
				<< std::setfill(' ')
				<< std::setw(5)
#endif
				<< tip
				<< " -- "
				<< msg
				;
			;
			return ss.str();
		}
	}
}
