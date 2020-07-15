/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SET_CAST_H
#define CYNG_SET_CAST_H

#include <cyng/intrinsics/sets.h>
#include <cyng/value_cast.hpp>

namespace cyng 
{
	/**
	 * @brief tuple to parameter map.
	 *
	 * Copy all elements in the tuple of type param_t 
	 * to the parameter map.
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
	 * The data types are not verified.
	 */
	inline vector_t to_vector(tuple_t const& tpl)	{
		return vector_t(tpl.begin(), tpl.end());
	}

	/**
	 * @brief copy tuple into vector
	 */
	inline tuple_t to_tuple(vector_t const& vec)	{
		return tuple_t(vec.begin(), vec.end());
	}

	/**
	 * Convinient value_cast<tuple_t>()
	 */
	tuple_t to_tuple(object obj);

	/**
	 * Convinient value_cast<vector_t>()
	 */
	vector_t to_vector(object obj);

	/**
	 * Convinient value_cast<param_map_t>()
	 */
	param_map_t to_param_map(object obj);

	/**
	 * Convinient value_cast<attr_map_t>()
	 */
	attr_map_t to_attr_map(object obj);

	/**
	 * Convinient value_cast<param_t>()
	 */
	param_t to_param(object obj);

	template <typename T >
	std::pair<std::string, T> to_pair(param_t param)
	{
		return std::make_pair(param.first, cyng::value_cast<T>(param.second));
	}

	/**
	 * Convinient value_cast<attr_t>()
	 */
	attr_t to_attr(object obj);

	template <typename T >
	std::pair<std::size_t, T> to_pair(attr_t param)
	{
		return std::make_pair(param.first, cyng::value_cast<T>(param.second));
	}

	/**
	 * @brief selects an entry from a parameter map and casts the
	 * the object to the specified value type.
	 *
	 * @return if key wasn't found or cast doesn't work it returns the
	 * default value.
	 */
	template< typename T >
	T from_param_map(param_map_t const& pm, std::string const& key, T def) {
		auto const pos = pm.find(key);
		if (pos != pm.end()) {
			return value_cast<T>(pos->second, def);
		}
		return def;
	}

	/**
	 * Convert all parameters of the parameter map into objects
	 * and copy these objects into a tuple of the same size.
	 */
	tuple_t to_tuple(param_map_t const& pm);


}

#endif 

