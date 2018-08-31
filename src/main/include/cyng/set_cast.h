/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SET_CAST_H
#define CYNG_SET_CAST_H

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
	 * @brief vector to parameter map.
	 *
	 * No check for duplicates
	 */
	param_map_t to_param_map(vector_t const&);

	/**
	 * @brief tuple to attribute map.
	 *
	 * No check for duplicates
	 */
	attr_map_t to_attr_map(tuple_t const&);

	/**
	 * @brief copy tuple into vector
	 *
	 * No check for duplicates
	 */
	vector_t to_vector(tuple_t const&);

	/**
	 * @brief copy tuple into vector
	 *
	 * No check for duplicates
	 */
	 tuple_t to_tuple(vector_t const&);

}

#endif 

