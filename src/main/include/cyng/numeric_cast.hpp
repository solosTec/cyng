/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_NUMERIC_CAST_HPP
#define CYNG_NUMERIC_CAST_HPP

#include <type_traits>
#include <limits.h>
#include <cyng/value_cast.hpp>
 
namespace cyng 
{
	/**
	 * If the hidden value is not of type T the function returns the default value.
	 *
	 * @tparam T cast object 
	 * @param def default value
	 * @return a const reference const& of the value or the default value if 
	 */
	template < typename T >
	T numeric_cast(object const& obj, T const& def) noexcept
	{
		static_assert(std::is_arithmetic<T>::value, "only arithmetic types supported");
		switch (obj.get_class().tag())	{
			
			case traits::TC_NULL:	return static_cast<T>(0);
			
			case traits::TC_BOOL:	
				return value_cast(obj, false) 
				? std::numeric_limits<T>::lowest()
				: std::numeric_limits<T>::max() 
				;
				
			case traits::TC_CHAR:	return static_cast<T>(value_cast<char>(obj, 0));
			case traits::TC_UINT8:	return static_cast<T>(value_cast<std::uint8_t>(obj, 0));
			case traits::TC_UINT16:	static_cast<T>(return value_cast<std::uint16_t>(obj, 0));
			case traits::TC_UINT32:	static_cast<T>(return value_cast<std::uint32_t>(obj, 0));
			case traits::TC_UINT64:	static_cast<T>(return value_cast<std::uint64_t>(obj, 0));
			case traits::TC_INT8:	static_cast<T>(return value_cast<std::int8_t>(obj, 0));
			case traits::TC_INT16:	static_cast<T>(return value_cast<std::int16_t>(obj, 0));
			case traits::TC_INT32:	static_cast<T>(return value_cast<std::int32_t>(obj, 0));
			case traits::TC_INT64:	static_cast<T>(return value_cast<std::int64_t>(obj, 0));
			
			case traits::TC_FLOAT:		return static_cast<T>(value_cast<float>(obj, 0.0));
			case traits::TC_DOUBLE:		return static_cast<T>(value_cast<double>(obj, 0.0));
			case traits::TC_FLOAT80:	return static_cast<T>(value_cast<long double>(obj, 0.0));

			default:
				break;
		}
		
		return static_cast<T>(0);
	}	
}

#endif //	CYNG_NUMERIC_CAST_HPP
