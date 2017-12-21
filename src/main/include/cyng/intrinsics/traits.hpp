/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TRAITS_HPP
#define CYNG_TRAITS_HPP

#include <cstddef>
#include <type_traits>
#include <limits>
#include <CYNG_project_info.h>

namespace cyng 
{
	namespace traits
	{
		/**
		 * specialize this struct for user defined data types
		 */
		template <typename T>
		struct type_tag
		{
			using type = T;
			using tag =  std::integral_constant<std::size_t, std::numeric_limits<std::size_t>::max()>;
#if defined(CYNG_LEGACY_MODE_ON)
			const static char name[];
#else
			//	C++17 allows constexpr string_view
			constexpr static char name[] = "user-defined";
#endif
		};
#if defined(CYNG_LEGACY_MODE_ON)
		template <typename T>
		const char type_tag<T>::name[] = "user-defined";
#endif
		
		
		enum /*class*/ predef_type_code : std::size_t
		{
			PREDEF_TABLE = 0x100,
			PREDEF_DB,
			PREDEF_KEY,
			PREDEF_VM_CALL,
		};
	}
}

#endif	//	CYNG_TRAITS_HPP


