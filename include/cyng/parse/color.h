/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_COLOR_H
#define CYNG_PARSE_COLOR_H

#include <cyng/obj/intrinsics/color.hpp>
#include <cyng/parse/string.h>
 
namespace cyng {

	/**
	 * expected format is "#rrggbboo" or "#rrrrggggbbbboooo"
	 * All numbers are hexadecimal.
	 */
	template < typename T >
	color<T> to_color(std::string const& str) {

		typename color<T>::rgb_type rgba{ 0 };

		switch (str.size()) {
		case 6:
		case 8:
			for (std::size_t offset = 0; offset < str.size(); offset += 2) {
				rgba.at(offset / 2) = to_numeric<T, 16>(str.substr(offset, 2));
			}
			break;
		case 12:
		case 16:
			for (std::size_t offset = 0; offset < str.size(); offset += 4) {
				rgba.at(offset / 4) = to_numeric<T, 16>(str.substr(offset, 4));
			}
			break;
		default:
			break;
		}
		return color<T>(rgba);
	}


}
#endif

