/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_CHRONO_HPP
#define CYNG_IO_CHRONO_HPP


#include <cyng/io.h>
#include <cyng/chrono.h>
#include <iostream>
#include <iomanip>
#include <boost/io/ios_state.hpp>

namespace cyng 
{	
	/**
	 * Write the time stamp in the following format:
	 * 
	 * @code
	 * yyyy-mm-dd hh:mm:ss.ssssss
	 * @endcode
	 * 
	 * All elements of fixed with and padded with zeros (0) if required
	 */
	std::ostream& operator<<(std::ostream& os, std::chrono::system_clock::time_point const&);
	std::ostream& operator<<(std::ostream& os, chrono::dbl_time_point const&);

	/**
	 * Write a timespan in hh::mm::ss.ffff format.
	 * 
	 * @tparam R an arithmetic type representing the number of ticks
	 * @tparam P a std::ratio representing the tick period (i.e. the number of seconds per tick)
	 */
	template <typename R, typename P>
	std::ostream& operator<<(std::ostream& os, std::chrono::duration<R, P> const& v)
	{
		const std::chrono::hours   h = std::chrono::duration_cast<std::chrono::hours>(v);
		const std::chrono::minutes  m = std::chrono::duration_cast<std::chrono::minutes>(v - h);
		const std::chrono::duration<float> s = v - h - m;

		boost::io::ios_flags_saver  ifs(os);
		
		os
			<< std::setfill('0')
			<< std::setw(2)
			<< h.count()
			<< ':'
			<< std::setw(2)
			<< (m.count() % 60ULL)	//	minutes
			<< ':'
			//	add fractional part of seconds
#if defined(_MSC_VER) && (_MSC_VER < 1700)
			<< std::defaultfloat	//	unknown modifier for GNU libstdc++ version 20150327
#else
			<< std::setprecision(6)
			<< std::fixed
#endif
			<< s.count()
			;
		return os;
	}
	
}

#endif // CYNG_IO_CHRONO_HPP
