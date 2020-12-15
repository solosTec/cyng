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

#include <boost/assert.hpp>

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

	/** @brief Copy an arithmetic type into an buffer
	 * 
	 * byte ordering ignored
	 */
	template <typename T>
	buffer_t to_buffer(T n) {

		static_assert(std::is_arithmetic_v<T>, "arithmetic type expected");

		buffer_t vec;
		vec.reserve(sizeof(T));

		auto const p = static_cast<const buffer_t::value_type*>(static_cast<const void*>(&n));
		BOOST_ASSERT(p != nullptr);

		std::copy(p, p + sizeof(T), std::back_inserter(vec));
		return vec;
	}
}

#endif 

