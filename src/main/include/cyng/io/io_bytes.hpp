/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_BYTES_HPP
#define CYNG_IO_BYTES_HPP


#include <cyng/io.h>
#include <iostream>
#include <iomanip>
#include <type_traits>
#include <boost/io/ios_state.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace cyng 
{	


	template <typename T>
	std::string bytes_to_str(T v)
	{
		static_assert(std::is_integral<T>::value, "integral required");
		static_assert(std::is_unsigned<T>::value, "unsigned required");

		const char* units[] = { "Bytes", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
		std::size_t idx = 0;
		auto d = boost::numeric_cast<double>(v);
		while ((d >= 1024) && (idx < 8)) {
			d /= 1024;
			idx++;
		}

		std::stringstream ss;
		ss.flags(std::ios::fixed);
		ss.precision(2);

		ss << std::dec << d << ' ' << units[idx];
		return ss.str();
	}

}

#endif 
