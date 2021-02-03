/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_BUFFER_H
#define CYNG_OBJ_INTRINSCIS_BUFFER_H

#include <vector>
#include <array>
#include <cstdint>
#include <string>

namespace cyng {

	/**
	 * Declare a buffer with contiguous memory
	 */
	using buffer_t = std::vector<char>;

	/**
	 * helper function to build a buffer from hex values
	 */
	buffer_t make_buffer(std::initializer_list<std::uint8_t> ilist);

	/**
	 * helper function to build a buffer from string
	 */
	buffer_t make_buffer(std::string const&);

	/**
	 * helper function to build a buffer from an array
	 */
	template <typename T, std::size_t N>
	buffer_t make_buffer(std::array<T, N> const& a)
	{
		return buffer_t(a.begin(), a.end());
	}

	/**
	 * helper function to build a buffer from an array
	 */
	template <typename T, std::size_t N>
	buffer_t make_buffer(T const(&v)[N])
	{
		return buffer_t(std::begin(v), std::end(v));
	}

	/** 
	 * @return true if all elements are printable ascii codes
	 */
	bool is_ascii(buffer_t const&);

	/**
	 * @return hash of the buffer content
	 */
	std::size_t hash(buffer_t const& buffer);
}

#include <functional>

namespace std {

	template <>
	class hash<cyng::buffer_t> {
	public:
		size_t operator()(cyng::buffer_t const& v) const;
	};
}

#endif 

