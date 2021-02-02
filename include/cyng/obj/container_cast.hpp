/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_CONTAINER_CAST_H
#define DOCC_OBJ_CONTAINER_CAST_H

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object_cast.hpp>
#include <iterator>

namespace docscript {

	/**
	 * Extract C from obj.
	 *
	 * Precondition: obj is of type C
	 * Postcondition: obj is empty
	 * 
	 * Example:
	 * @code
	 * auto obj = docscript::make_object(docscript::make_tuple(1, 2, 3.4));
     * auto tpl = docscript::container_cast<docscript::tuple_t>(obj);
	 * @endcode
	 */
	template <typename C >
	[[nodiscard]]
	C container_cast(object& obj) {
		auto r = object_release<C>(obj);
		return (r.first != nullptr)
			? *(r.first)
			: C{}
		;
	}

	/**
	 * Create a copy.
	 *
	 * Precondition: obj is of type C
	 * Postcondition: obj is unchanged
	 * 
	 * Example:
	 * @code
	 * auto const obj = docscript::make_object(docscript::make_tuple(1, 2, 3.4));
	 * auto tpl = docscript::container_cast<docscript::tuple_t>(obj);
	 * @endcode
	 */
	template <typename C >
	[[nodiscard]]
	C container_cast(object const& obj) {
		auto const* p = object_cast<C>(obj);
		return (p != nullptr)
			? *p	//	copy
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
	 * auto vec = container_transform<docscript::vector_t>(docscript::make_tuple(1, 2, 3.4));
	 * @endcode
	 */
	template <typename C2, typename C1 >
	[[nodiscard]]
	C2 container_transform(C1&& c) {
		return { std::make_move_iterator(std::begin(c)), std::make_move_iterator(std::end(c)) };
	}

}

#endif //	DOCC_OBJ_CONTAINER_CAST_H
