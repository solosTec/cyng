/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJECT_CAST_HPP
#define CYNG_OBJECT_CAST_HPP

#include <cyng/core/wrapper.hpp>
 
namespace cyng 
{
	/**
	 * If the hidden value is not of type T the function returns nullptr.
	 *
	 * @tparam T cast object 
	 * @return constant pointer to the value hosted by the cyng-object.
	 */
	template < typename T >
	const T* object_cast(object const& obj) noexcept
	{
// 		using type = typename std::decay< T >::type
		using value_type = core::wrapper< T >;

		//	cast to the (hopefully) correct shared value pointer
		auto p = std::dynamic_pointer_cast< value_type >(obj.value_);
		
		//	if dynamic cast failed we return a null pointer
		return (!p)
		? nullptr
		: &p->held_
		;
	}	
}

#endif //	CYNG_OBJECT_CAST_HPP
