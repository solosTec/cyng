/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_BUFFER_H
#define CYNG_INTRINSICS_BUFFER_H

#include <vector>

namespace cyng 
{
	/**
	 * Declare a buffer with contiguous memory
	 */
	using buffer_t = std::vector<char>;

	/**
	 * helper function to build a buffer from hex values
	 */
	buffer_t make_buffer(std::initializer_list<std::uint8_t> ilist);
}

#endif 	// CYNG_INTRINSICS_BUFFER_H

