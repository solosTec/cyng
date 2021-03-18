/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_ADD_HPP
#define CYNG_OBJ_ALGORITHM_ADD_HPP

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/container.h>

#include <iterator>

namespace cyng {

	/**
	 * Add one container to another
     */
	template <typename T, template<typename, typename = std::allocator<T>> class C>
	void add(C<T>& target, C<T> const& source) {
		target.insert(target.end(), source.begin(), source.end());
	}

	/**
	 * rvalues are moved
	 */
	template <typename T, template<typename, typename = std::allocator<T>> class C>
	void add(C<T>& target, C<T>&& source) {
		target.insert(target.end(), std::make_move_iterator(source.begin()), std::make_move_iterator(source.end()));
	}
}

#endif 
