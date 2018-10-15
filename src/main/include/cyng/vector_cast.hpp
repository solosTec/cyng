/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VECTOR_CAST_HPP
#define CYNG_VECTOR_CAST_HPP

#include <cyng/value_cast.hpp>
#include <cyng/intrinsics/sets.h>
 
namespace cyng 
{
	template < typename T >
	std::vector< T > vector_cast(cyng::vector_t const& vec, T const& def) noexcept
	{
		std::vector< T > result;
		for (auto const& v : vec) {
			result.push_back(value_cast<T>(v, def));
		}
		return result;
	}

	/**
	 * Read a vector of the specified data type.
	 * 
	 * example
	 * @code
	  std::vector<std::string> vec = vector_cast<std::string>(obj, "");
	 * @endcode
	 */
	template < typename T >
	std::vector< T > vector_cast(object const& obj, T const& def) noexcept
	{
		vector_t vec;
		return vector_cast<T>(value_cast(obj, vec), def);
	}	

}

#endif //	CYNG_VALUE_CAST_HPP

