/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJECT_CAST_FWD_HPP
#define CYNG_OBJECT_CAST_FWD_HPP
 
#include <cyng/core/object_interface_fwd.h>
 
namespace cyng 
{
	template < typename T >
	const T* object_cast(object const&) noexcept;
}

#endif //	CYNG_OBJECT_CAST_FWD_HPP
