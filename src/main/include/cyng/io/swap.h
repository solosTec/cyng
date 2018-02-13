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
	std::uint16_t swap_num(std::uint16_t val);

	/**
	 *	Byte swap short
	 */
	std::int16_t swap_num(std::int16_t val);

	/**
	 *	Byte swap unsigned int
	 */
	std::uint32_t swap_num(std::uint32_t val);

	/**
	 *	Byte swap int
	 */
	std::int32_t swap_num(std::int32_t val);
	std::int64_t swap_num(std::int64_t val);
	std::uint64_t swap_num(std::uint64_t val);
}

#endif // CYNG_IO_SWAP_H
