/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SET_CAST_HPP
#define CYNG_SET_CAST_HPP

#include <cyng/intrinsics/sets.h>
 

namespace cyng 
{
	/**
	 * @brief tuple to parameter map.
	 *
	 * No check for duplicates
	 */
	param_map_t to_param_map(tuple_t const&);

	/**
	 * @brief tuple to attribute map.
	 *
	 * No check for duplicates
	 */
	attr_map_t to_attr_map(tuple_t const&);
}

#endif //	CYNG_SET_CAST_HPP

