/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_BUFFER_CAST_HPP
#define CYNG_OBJ_BUFFER_CAST_HPP

#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/object.h>

#include <algorithm>
//#include <bit>
#include <cyng.h>	//	docc_BIG_ENDIAN
#include <cstring>
#include <array>

#if defined(max)
#undef max
#endif

#if defined(min)
#undef min
#endif

#include <boost/assert.hpp>

namespace cyng {


	/**
	 * Convinient value_cast<buffer_t>()
	 * Not suited for large buffer sizes.
	 * 
	 * Precondition: obj is of type "binary"
	 */
	[[nodiscard]]
	buffer_t to_buffer(object const& obj);

	[[nodiscard]]
	buffer_t to_buffer(std::string const& str);

	/**
	 * Copy the buffer into a string but terminates copying
	 * when a 0 value occurs.
	 */
	[[nodiscard]]
	std::string to_string_nil(buffer_t const& str, std::size_t offset);

	/**
	 * Convert content of an buffer into a numeric data type
	 * as big-endian.
	 */
	template <typename T>
	[[nodiscard]]
	T to_numeric_be(buffer_t const& buffer) {
		T r = T();
		auto const size = std::min(sizeof(T), buffer.size());

		std::memcpy(&r, buffer.data(), size);
		return r;
	}

	/**
	 * Convert content of an buffer into a numeric data type
	 * as little-endian.
	 */
	template <typename T>
	[[nodiscard]]
	T to_numeric_le(buffer_t const& buffer) {

		T r = T();
		auto const size = std::min(sizeof(T), buffer.size());

		buffer_t src;
		src.resize(size);
		std::reverse_copy(buffer.begin(), buffer.begin() + size, src.begin());
		std::memcpy(&r, src.data(), size);
		return r;
	}

	/**
	 * Convert content of an buffer into a numeric data type
	 * as big-endian. Reverse buffer before calling this function
	 * to get a little-endian value.
	 */
	template <typename T>
	[[nodiscard]]
	T to_numeric(buffer_t const& buffer) {

		//
		//	check network byte ordering
		//
#if defined(docc_BIG_ENDIAN)
		return to_numeric_be<T>(buffer);
#else
		return to_numeric_le<T>(buffer);
#endif
	}

	template <typename T>
	[[nodiscard]]
	T to_numeric_be(buffer_t const& buffer, std::size_t offset) {

		T r = T();
		auto const size = std::min(sizeof(T), buffer.size());
		BOOST_ASSERT(sizeof(T) <= buffer.size() - offset);

		std::memcpy(&r, buffer.data() + offset, size);
		return r;
	}

	template <typename T>
	[[nodiscard]]
	T to_numeric_le(buffer_t const& buffer, std::size_t offset) {

		T r = T();
		auto const size = std::min(sizeof(T), buffer.size());
		BOOST_ASSERT(sizeof(T) <= buffer.size() - offset);

		buffer_t src;
		src.resize(sizeof(T));
		auto const pos = buffer.begin() + offset;
		std::reverse_copy(pos, pos + size, src.begin());
		std::memcpy(&r, src.data(), sizeof(T));
		return r;
	}

	template <typename T>
	[[nodiscard]]
	T to_numeric(buffer_t const& buffer, std::size_t offset) {

		//
		//	check network byte ordering
		//
#if defined(docc_BIG_ENDIAN)
		return to_numeric_be<T>(buffer, offset);
#else
		return to_numeric_le<T>(buffer, offset);
#endif

	}

	/** @brief Copy an arithmetic type into an buffer
	 *	big endian
	 */
	template <typename T, std::size_t N = sizeof(T), std::size_t OFFSET = 0>
	[[nodiscard]]
	buffer_t to_buffer_be(T n) {

		static_assert(OFFSET <= N, "index out if range");
		static_assert(N + OFFSET <= sizeof(T), "type size exceeded");
		static_assert(std::is_trivial<T>::value, "trivial data type required");

		using length_t = std::integral_constant<std::size_t, N - OFFSET>;

		buffer_t vec;
		vec.resize(length_t::value);
		BOOST_ASSERT(vec.size() == length_t::value);

		if (vec.size() == length_t::value) {
			auto r = vec.data();
			const void* src = reinterpret_cast<const char*>(&n) + OFFSET;
			std::memcpy(r, src, length_t::value);
		}
		return vec;
	}

	/** @brief Copy an arithmetic type into an buffer
	 *
	 */
	template <typename T, std::size_t N = sizeof(T), std::size_t OFFSET = 0>
	[[nodiscard]]
	buffer_t to_buffer(T n) {

		auto buffer = to_buffer_be<T, N, OFFSET>(n);
		//
		//	convert to network byte ordering
		//
#if !defined(docc_BIG_ENDIAN)
		std::reverse(buffer.begin(), buffer.end());
#endif
		return buffer;
	}

	/**
	 * @tparam T The type of the elements.
	 * @tparam N number of elements to copy
	 * @tparam OFFSET index in array to start with copying
	 */
	template < typename T, std::size_t N, std::size_t OFFSET = 0 >
	[[nodiscard]]
	buffer_t to_buffer(std::array< T, N > const& a) {

		using array_type = std::array< T, N >;
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");
		static_assert(N + OFFSET == std::tuple_size<array_type>::value, "invalid template parameters");
		static_assert(OFFSET <= N, "index out if range");

		using length_t = std::integral_constant<std::size_t, (N - OFFSET) * sizeof(T) >;

		buffer_t vec;
		vec.resize(length_t::value);
		if (vec.size() == length_t::value) {

			auto r = vec.data();
			std::memcpy(r, &a.at(OFFSET), length_t::value);

		}
		return vec;
	}

	template < typename T, std::size_t N, std::size_t OFFSET = 0 >
	[[nodiscard]]
	buffer_t to_buffer(T const(&p)[N]) {

		using length_t = std::integral_constant<std::size_t, (N - OFFSET) * sizeof(T) >;
		static_assert(OFFSET <= N, "index out if range");

		buffer_t vec;
		vec.resize(length_t::value);
		if (vec.size() == length_t::value) {

			auto r = vec.data();
			std::memcpy(r, &p[OFFSET], length_t::value);

		}
		return vec;
	}

}

#endif //	CYNG_OBJECT_CAST_HPP
