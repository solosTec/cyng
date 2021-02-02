/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_COMPARE_HPP
#define DOCC_OBJ_COMPARE_HPP

#include <cyng/obj/value_cast.hpp>

namespace docscript {

	/**
	 * Compare an object with a specified value.
	 * The object must contain the same data type as
	 * the value.
	 */
	template <typename T>
	bool compare(object const& obj, T v) {
		
		BOOST_ASSERT(is_same_type<T>(obj));

		return (is_same_type<T>(obj))
			? (v == value_cast<T>(obj, v))
			: false
			;
	}

	/**
	 * Provide a functor to compare an object
	 * with an value.
	 */
	template <typename T>
	struct comparator
	{
		comparator(T const& v)
			: value_(v)
		{}

		bool operator()(object const& obj) const {
			return compare<T>(obj, value_);
		}

		T const value_;
	};
}

#endif //	DOCC_OBJ_VECTOR_CAST_H
