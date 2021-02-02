/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_BUFFER_CAST_HPP
#define DOCC_OBJ_BUFFER_CAST_HPP

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

namespace docscript {


	/**
	 * Convinient value_cast<buffer_t>()
	 * Not suited for large buffer sizes.
	 * 
	 * Precondition: obj is of type "binary"
	 */
	[[nodiscard]]
	buffer_t to_buffer(object const& obj);

	/**
	 * Convert content of an buffer into a numeric data type
	 * as big-endian. Reverse buffer before calling this function
	 * to get a little-endian value.
	 */
	template <typename T>
	[[nodiscard]]
	T to_numeric(buffer_t const& buffer) {

		T r = T();
		auto const size = std::min(sizeof(T), buffer.size());

		//
		//	check network byte ordering
		//
#if defined(docc_BIG_ENDIAN)
		std::memcpy(&r, buffer.data(), size);
#else
		buffer_t src;
		src.resize(size);
		std::reverse_copy(buffer.begin(), buffer.begin() + size, src.begin());
		std::memcpy(&r, src.data(), size);
#endif
		// if constexpr (std::endian::native == std::endian::little) {
		// 	buffer_t src;
		// 	src.resize(size);
		// 	std::reverse_copy(buffer.begin(), buffer.begin() + size, src.begin());
		// 	std::memcpy(&r, src.data(), size);
		// }
		// else {
		// 	std::memcpy(&r, buffer.data(), size);
		// }

		return r;
	}

	template <typename T>
	[[nodiscard]]
	T to_numeric(buffer_t const& buffer, std::size_t offset) {

		T r = T();
		auto const size = std::min(sizeof(T) + offset, buffer.size());
		BOOST_ASSERT(sizeof(T) == buffer.size() - offset);

		//
		//	check network byte ordering
		//
#if defined(docc_BIG_ENDIAN)
		std::memcpy(&r, buffer.data() + offset, size);
#else
		buffer_t src;
		src.resize(sizeof(T));
		std::reverse_copy(buffer.begin() + offset, buffer.begin() + size, src.begin());
		std::memcpy(&r, src.data(), sizeof(T));
#endif
		//if constexpr (std::endian::native == std::endian::little) {
		//	buffer_t src;
		//	src.resize(sizeof(T));
		//	std::reverse_copy(buffer.begin() + offset, buffer.begin() + size, src.begin());
		//	std::memcpy(&r, src.data(), sizeof(T));
		//}
		//else {
		//	std::memcpy(&r, buffer.data() + offset, size);
		//}

		return r;

	}

	/** @brief Copy an arithmetic type into an buffer
	 *
	 */
	template <typename T, std::size_t N = sizeof(T), std::size_t OFFSET = 0>
	[[nodiscard]]
	buffer_t to_buffer(T n) {

		static_assert(OFFSET <= N, "index out if range");
		static_assert(N + OFFSET <= sizeof(T), "type size exceeded");

		using length_t = std::integral_constant<std::size_t, N - OFFSET>;

		buffer_t vec;
		vec.resize(length_t::value);
		BOOST_ASSERT(vec.size() == length_t::value);

		if (vec.size() == length_t::value) {
			auto r = vec.data();
			const void* src = reinterpret_cast<const char*>(&n) + OFFSET;
			std::memcpy(r, src, length_t::value);

			//
			//	check network byte ordering
			//
#if !defined(docc_BIG_ENDIAN)
			std::reverse(vec.begin(), vec.end());
#endif

			//if constexpr (std::endian::native == std::endian::little) {
			//	std::reverse(vec.begin(), vec.end());
			//}

		}
		return vec;
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
