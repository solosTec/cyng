/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_HEX_H
#define CYNG_PARSE_HEX_H

#include <cstdint>
#include <string>

namespace cyng {
	std::uint8_t hex_to_int(char c);
	std::uint8_t hex_to_int(char ch, char cl);

	/**
	 * @brief convert a hex string into a unsigned int
	 * 
	 * @param s expected length is 2
	 * @return std::uint8_t 
	 */
	std::uint8_t hex_to_int(std::string s);
}
#endif

