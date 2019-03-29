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
#if defined(_CYNG_CPP_SUPPORT_N2235)
			//	C++17 allows constexpr string_view
			constexpr static char name[] = "user-defined";
#else
			const static char name[];
#endif
		};

#if !defined(_CYNG_CPP_SUPPORT_N2235)
		template <typename T>
		const char type_tag<T>::name[] = "user-defined";
#endif
		
		
		enum 
#if defined(_CYNG_CPP_SUPPORT_N2347)
		class 
#endif
		predef_type_code: std::size_t
		{
			PREDEF_TABLE = 0x100,
			PREDEF_DB,
			PREDEF_VM_CALL,
			PREDEF_SESSION,
			PREDEF_SSL_SESSION,
			PREDEF_CONNECTION,
			PREDEF_SOCKET,	//	e.g. websocket
			PREDEF_SSL_SOCKET,
			PREDEF_SK,
			PREDEF_CUSTOM,
		};
	}
}

#endif	


