/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_BUFFER_H
#define CYNG_INTRINSICS_BUFFER_H

#include <vector>
#include <array>
#include <cstdint>

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

	/**
	 * helper function to build a buffer from an array
	 */
	template <typename T, std::size_t N>
	buffer_t make_buffer(std::array<T, N> const& a)
	{
		return buffer_t(a.begin(), a.end());
	}

	/** 
	 * @return true if all elements are printable ascii codes
	 */
	bool is_ascii(buffer_t const&);
}

#endif 

