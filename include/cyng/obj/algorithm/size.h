/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_SIZE_H
#define CYNG_OBJ_ALGORITHM_SIZE_H

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/container.h>
//#include <cyng/obj/value_cast.hpp>
//#include <cyng/obj/numeric_cast.hpp>

#include <type_traits>

namespace cyng {

	/**
	 * Get the logical size of container classes
	 */
	std::size_t size(vector_t const&);
	std::size_t size(tuple_t const&);
	std::size_t size(deque_t const&);
	std::size_t size(attr_map_t const&);
	std::size_t size(param_map_t const&);
	std::size_t size(prop_map_t const&);
	std::size_t size(object const&);

}

#endif //	CYNG_OBJ_ALGORITHM_FIND_H
