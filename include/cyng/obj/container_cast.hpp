/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_CONTAINER_CAST_H
#define CYNG_OBJ_CONTAINER_CAST_H

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object_cast.hpp>
#include <iterator>

namespace cyng {

	/**
	 * Extract C from obj.
	 *
	 * Precondition: obj is of type C
	 * Postcondition: obj is empty
	 * 
	 * Example:
	 * @code
	 * auto obj = cyng::make_object(cyng::make_tuple(1, 2, 3.4));
     * auto tpl = cyng::container_cast<cyng::tuple_t>(obj);
	 * @endcode
	 */
	//template <typename C >
	//[[nodiscard]]
	//C container_cast(object& obj) {
	//	auto r = object_release<C>(obj);
	//	if (r.first != nullptr) {
	//		? *(r.first)
	//	}
	//	return C{};
	//}

	/**
	 * Create a copy.
	 *
	 * Precondition: obj is of type C
	 * Postcondition: obj is unchanged
	 * 
	 * Example:
	 * @code
	 * auto const obj = cyng::make_object(cyng::make_tuple(1, 2, 3.4));
	 * auto tpl = cyng::container_cast<cyng::tuple_t>(obj);
	 * @endcode
	 */
	template <typename C >
	[[nodiscard]]
	C container_cast(object const& obj) {
		auto const* ptr = object_cast<C>(obj);
		return (ptr != nullptr)
			? *ptr	//	copy
			: C{}
		;
	}

	/**
	 * @brief copy C1 into C2
	 *
	 * The data types are not verified.
	 */
	template <typename C2, typename C1 >
	[[nodiscard]]
	C2 container_transform(C1 const& c) {
		return { std::begin(c), std::end(c) };
	}

	/**
	 * @brief move C1 into C2
	 *
	 * The data types are not verified.
	 */
	template <typename C2, typename C1 >
	[[nodiscard]]
	C2 container_transform(C1& c) {
		return { std::make_move_iterator(std::begin(c)), std::make_move_iterator(std::end(c)) };
	}

	/**
	 * @brief move C1 into C2
	 *
	 * The data types are not verified.
	 *
	 * Example:
	 * @code
	 * auto vec = container_transform<cyng::vector_t>(cyng::make_tuple(1, 2, 3.4));
	 * @endcode
	 */
	template <typename C2, typename C1 >
	[[nodiscard]]
	C2 container_transform(C1&& c) {
		return { std::make_move_iterator(std::begin(c)), std::make_move_iterator(std::end(c)) };
	}

	/**
	 * convert a tuple of "param_t"s to a parameter map.
	 */
	param_map_t to_param_map(tuple_t const& tpl);

	//param_map_t to_param_map(vector_t const& vec);


}

#endif //	CYNG_OBJ_CONTAINER_CAST_H
