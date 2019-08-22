/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_SWAP_H
#define CYNG_IO_SWAP_H

#include <cstdint>

namespace cyng 
{

	/**
	 *	Byte swap unsigned short
	 */
	constexpr std::uint16_t swap_num(std::uint16_t val) {
		return (val << 8) | (val >> 8);
	}

	/**
	 *	Byte swap short
	 */
	constexpr std::int16_t swap_num(std::int16_t val) {
		return (val << 8) | ((val >> 8) & 0xFF);
	}

	/**
	 *	Byte swap unsigned int
	 */
	constexpr std::uint32_t swap_num(std::uint32_t val) {
		val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
		return (val << 16) | (val >> 16);
	}

	/**
	 *	Byte swap int
	 */
	constexpr std::int32_t swap_num(std::int32_t val) {
		val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
		return (val << 16) | ((val >> 16) & 0xFFFF);
	}

	constexpr std::int64_t swap_num(std::int64_t val) {
		val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
		val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
		return (val << 32) | ((val >> 32) & 0xFFFFFFFFULL);
	}

	constexpr std::uint64_t swap_num(std::uint64_t val) {
		val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
		val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
		return (val << 32) | (val >> 32);
	}
}

#endif
