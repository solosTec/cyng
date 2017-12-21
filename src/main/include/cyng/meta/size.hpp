/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_META_SIZE_HPP
#define CYNG_META_SIZE_HPP

#include <cyng/meta/meta.hpp>

namespace cyng 
{
	namespace meta 
	{
		template<typename ...Args> 
		struct size_impl
		{
			using type = std::integral_constant<std::size_t, sizeof...(Args)>;
		};
		
		/**
		 * Specialized for templates
		 */
		template<template<class...> class L, class... T> 
		struct size_impl<L<T...>>
		{
			//	expand parameter pack
			using type = invoke<size_impl<T...>>;
		};
		
		/**
		 * Calculate size of a parameter pack. Similiar
		 * to std::tuple_size<T>
		 */
		template<typename ...L> 
		using size = typename size_impl<L...>::type;
		
	}
}


#endif	//	CYNG_META_SIZE_HPP


