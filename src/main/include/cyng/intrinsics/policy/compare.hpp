/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_POLICY_COMPARE_HPP
#define CYNG_POLICY_COMPARE_HPP



#include <cyng/core/object_interface_fwd.h>
#include <functional>

namespace std
{
	/**
	 * Don't compare the values!
	 */
	template <>
	struct less<cyng::object>
	{
		typedef cyng::object type;
		
		/**
		 * @return true of lhs pointer is less than rhs
		 */
		bool operator()( const type& lhs, const type& rhs) const;
	};
	
}

#endif 	// CYNG_POLICY_COMPARE_HPP

