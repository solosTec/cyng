/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_CLONE_HPP
#define CYNG_OBJ_CLONE_HPP

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/container_cast.hpp>


namespace cyng {

	/**
	 * Clone one container into another of the same type
	 */
	template <typename T>
	[[nodiscard]]
	T clone(T const& c) {
		return container_transform<T, T>(c);
	}

}

#endif 
