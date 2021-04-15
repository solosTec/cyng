/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ARRAY_CAST_H
#define CYNG_OBJ_ARRAY_CAST_H

#include <array> 
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <iterator>

#ifdef _DEBUG
#include <cyng/io/ostream.h>
#include <cyng/io/ostream.h>
#endif

#include <boost/assert.hpp>

namespace cyng {

	/**
	 * Convert an integral type into an array of smaller integral types
	 */
	template <typename U, typename T>
	auto to_array(T n) -> std::array< U, sizeof(T) / sizeof(U) >
	{
		static_assert(sizeof(T) > sizeof(U), "subtype must be smaller");
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");

		//
		//	define an array to hold the integral type
		//
		using result_type = std::array< U, sizeof(T) / sizeof(U) >;

		auto const begin = reinterpret_cast<U const*>(&n);
		auto end = begin + sizeof(T);

		result_type a;

#if defined(cyng_BIG_ENDIAN)
		std::copy(begin, end, a.begin());
#else
		//	requires C++17 (N3291)
		std::reverse_copy(begin, end, a.begin());
#endif

		//
		//	use this if reverse_copy() isn't available
		//
		//	auto dest = a.begin();
		//	while (begin != end) {
		//		*(dest++) = *(--end);
		//	}
		//

		return a;

	}
}

#endif
