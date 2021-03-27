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
#include <cyng/obj/value_cast.hpp>

#include <iterator>
#include <algorithm>

namespace cyng {

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

	template <typename C >
	[[nodiscard]]
	C container_cast(object&& obj) {
		//	std::pair<T*, obj_interface*>
		auto rp = object_release<C>(obj);
		return (rp.first != nullptr)
			? std::move(*(rp.first))	//	move
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
	 * Use this if container elements are moveable.
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

	/**
	 * Convert a param_map_t to a std::map<>. Works for parameter maps with a homogenous
	 * second data type.
	 */
	template <typename T>
	auto to_map(param_map_t const& pm, T def) -> std::map<std::string, T> {
		std::map<std::string, T> r;
		std::transform(pm.begin(), pm.end(), std::inserter(r, r.end()), [def](param_map_t::value_type const& val) {

			return std::make_pair(val.first, cyng::value_cast<T>(val.second, def));

			});

		return r;
	}


}

#endif //	CYNG_OBJ_CONTAINER_CAST_H
