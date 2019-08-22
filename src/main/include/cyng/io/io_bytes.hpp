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

	template<typename CharT, typename Traits, typename T>
	inline std::basic_ostream<CharT, Traits >& bytes_to_str(std::basic_ostream<CharT, Traits>& os, T v)
	{
		static_assert(std::is_integral<T>::value, "integral required");
		static_assert(std::is_unsigned<T>::value, "unsigned required");

		//	store and reset stream state
		boost::io::ios_flags_saver  ifs(os);

		static const char* units[] = { "Bytes", "kB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };
		std::size_t idx = 0;
		auto d = boost::numeric_cast<double>(v);
		while ((d >= 1024) && (idx < 8)) {
			d /= 1024;
			idx++;
		}

		if (idx > 0) {
			os.flags(std::ios::fixed);
			os.precision(2);
		}

		os << std::dec << d << ' ' << units[idx];
		return os;
	}


	template <typename T>
	std::string bytes_to_str(T v)
	{
		std::stringstream ss;
		bytes_to_str(ss, v);
		return ss.str();
	}

}

#endif 
