/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VALUE_CAST_HPP
#define CYNG_VALUE_CAST_HPP

#include <cyng/core/wrapper.hpp>
 
#ifdef __GNUC__
#include <features.h>
#if ! __GNUC_PREREQ(5,0)
#pragma GCC diagnostic ignored "-Wreturn-local-addr"
#endif
#endif

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
	T value_cast(object const& obj, T const& def) noexcept
	{
		using value_type = core::wrapper< T >;

		//	cast to the (hopefully) correct shared value pointer
		auto p = std::dynamic_pointer_cast< value_type >(obj.value_);
		
		return (!p)
		? def
		: p->held_
		;
	}	
}

#endif //	CYNG_VALUE_CAST_HPP
