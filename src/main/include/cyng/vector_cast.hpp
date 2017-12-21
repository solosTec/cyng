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
 
// #ifdef __GNUC__
// #include <features.h>
// #if ! __GNUC_PREREQ(5,0)
// #pragma GCC diagnostic ignored "-Wreturn-local-addr"
// #endif
// #endif

namespace cyng 
{
	/**
	 * Read a vector of the specified data type.
	 * 
	 * example
	 * @code
	  std::vector<std::string> vec = vector_cast<int>(obj);
	 * @endcode
	 */
	template < typename T >
	T vector_cast(object const& obj, T const& def) noexcept
	{
		std::vector< T > result;
		const vector_t vec;
		vec = value_cast(obj, vev);
		
		for (v : vec)
		{
			result.push_back(value_cast<T>(v, def));
		}
		return result;
	}	
}

#endif //	CYNG_VALUE_CAST_HPP

