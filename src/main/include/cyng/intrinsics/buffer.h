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
}

#endif 	// CYNG_INTRINSICS_BUFFER_H

