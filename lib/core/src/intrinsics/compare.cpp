/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/object.h>
#include <cyng/intrinsics/policy/compare.hpp>

namespace std
{
// 	template <>
	bool less<cyng::object>::operator()( const type& obj1, const type& obj2) const
	{
		return obj1.value_.get() < obj2.value_.get();
	}
	
}



