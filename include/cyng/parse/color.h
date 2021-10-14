/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_COLOR_H
#define CYNG_PARSE_COLOR_H

#include <cyng/obj/intrinsics/color.hpp>
//#include <cyng/parse/buffer.h>
#include <cyng/parse/string.h>
 
namespace cyng {

	/**
	 * expected format is "#rrggbboo" or "#rrrrggggbbbboooo"
	 * All numbers are hexadecimal.
	 */
	template < typename T >
	color<T> to_color(std::string const& str) {
		typename color<T>::rgb_type rgba{ 0 };
		if (str.size() == 9 && str.at(0) == '#') {
			for (std::size_t offset = 1; offset < str.size(); offset += 2) {
				rgba.at((offset-1)/2) = to_numeric<T, 16>(str.substr(offset, 2));
			}
			return color<T>(rgba);
		}
		else if (str.size() == 17 && str.at(0) == '#') {
			for (std::size_t offset = 1; offset < str.size(); offset += 4) {
				rgba.at((offset - 1) / 4) = to_numeric<T, 16>(str.substr(offset, 4));
			}
			return color<T>(rgba);
		}
		return color<T>();
	}


}
#endif

