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

	/**
	 * Convert hex characters to an integer as big endian
	 */
	std::uint8_t hex_to_u8_be(char ch, char cl);

	/**
	 * Convert hex characters to an integer as little endian
	 */
	std::uint8_t hex_to_u8_le(char ch, char cl);

	/**
	 * Convert hex characters to an integer specified
	 * by the host system
	 */
	std::uint8_t hex_to_u8(char ch, char cl);

	/**
	 * @brief convert a hex string into a unsigned int
	 * 
	 * @param s expected length is 2
	 * @return std::uint8_t 
	 */
	std::uint8_t hex_to_u8(std::string s);

	/**
	 * Convert hex characters to an integer as big endian
	 */
	std::uint16_t hex_to_u16_be(char ch1, char c2, char c3, char c4);

	/**
	 * Convert hex characters to an integer as little endian
	 */
	std::uint16_t hex_to_u16_le(char ch1, char c2, char c3, char c4);

	std::uint16_t hex_to_u16(char ch1, char c2, char c3, char c4);
	std::uint16_t hex_to_u16(std::string s);
}
#endif

