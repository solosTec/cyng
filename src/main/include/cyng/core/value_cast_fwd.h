/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_VALUE_CAST_FWD_HPP
#define CYNG_VALUE_CAST_FWD_HPP
 
#include <cyng/core/object_interface_fwd.h>
 
namespace cyng 
{
	template < typename T >
	T value_cast(object const&, T const&) noexcept;
}

#endif //	CYNG_VALUE_CAST_FWD_HPP
