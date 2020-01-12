/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_BUFFER_CAST_H
#define CYNG_BUFFER_CAST_H

#include <cyng/intrinsics/buffer.h>
#include <cyng/object.h>
#include <algorithm>

namespace cyng 
{

	/**
	 * Convinient value_cast<buffer_t>()
     * Not suited for large buffer sizes.
	 */
	buffer_t to_buffer(object obj);

	/**
	 * Convert content of an buffer into a numeric data type
	 * as big-endian. Reverse buffer before calling this function 
	 * to get a little-endian value.
	 */
    template <typename T>
    T to_numeric(buffer_t buffer) {
		union {
			unsigned char source_[sizeof(T)];
			T l_;
		} u_;
		u_.l_ = T();
		auto const size = std::min(sizeof(T), buffer.size());
		std::copy_n(buffer.begin(), size, std::begin(u_.source_));
		return u_.l_;
	}
}

#endif 

