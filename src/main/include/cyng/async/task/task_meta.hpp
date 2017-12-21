/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_ASYNC_TASK_META_HPP
#define CYNG_ASYNC_TASK_META_HPP

#include <cyng/meta/conversion.hpp>
#include <cyng/meta/meta.hpp>
#include <cyng/intrinsics/sets.h>
#include <boost/assert.hpp>

namespace cyng 
{
	namespace async 
	{
		namespace detail 
		{
			template <typename T>
			struct functor
			{
				T& tsk_;
				functor(T& tsk)
				: tsk_(tsk)
				{}
				
				template <typename ...Args>
				void operator()(Args&&...  args)
				{
					tsk_.process(std::forward<Args>(args)...);
				}
			};
			
			template <std::size_t N, typename ...Args>
			struct select_signature_impl
			{
				//
				//	extract signature
				//
				using signature_t = typename std::tuple_element<N, std::tuple<Args...>>::type;
				
				//
				//	converter from tuple_t to std::tuple<...>
				//
				using converter_t = meta::converter_factory<signature_t>;
				static_assert(std::is_same<signature_t, typename converter_t::return_type>::value, "invalid message type");
				
				//
				//	select matching slot and call task method process(...)
				//
				template < typename TSK >
				static void invoke(TSK& tsk, std::size_t slot, tuple_t const& msg)
				{
					std::cout << "visit slot " << N << std::endl;
					if (N == slot)
					{
						std::cout << "MATCH " << slot << std::endl;
						typename converter_t::return_type tpl = converter_t::copy(msg.begin(), msg.end());
						functor<TSK> f(tsk);
						meta::apply(f, tpl);
// 						meta::apply([&tsk](auto &&... args){
// 							tsk.process(args...);
// 						}, tpl);
					}
					else 
					{
						select_signature_impl<N - 1, Args...>::invoke(tsk, slot, msg);
					}
				}
			};
			
			template <typename ...Args>
			struct select_signature_impl<0, Args...>
			{
				//
				//	extract signature
				//
				using signature_t = typename std::tuple_element<0, std::tuple<Args...>>::type;
				
				//
				//	converter from tuple_t to std::tuple<...>
				//
				using converter_t = meta::converter_factory<signature_t>;
				static_assert(std::is_same<signature_t, typename converter_t::return_type>::value, "invalid message type");
				
				template < typename TSK >
				static void invoke(TSK& tsk, std::size_t slot, tuple_t const& msg)
				{
					std::cout << "visit slot 0" << std::endl;
					BOOST_ASSERT_MSG(slot == 0, "slot error");
					std::cout << "MATCH " << slot << std::endl;
					typename converter_t::return_type tpl = converter_t::copy(msg.begin(), msg.end());
// #if defined(CYNG_LEGACY_MODE_ON)
					functor<TSK> f(tsk);
					meta::apply(f, tpl);
// #else
// 					meta::apply([&tsk](auto &&... args){
// 						tsk.process(args...);
// 					}, tpl);
// #endif
				}
			};

		}	//	detail
		
		/**
		 * Get a tuple element by index at runtime.
		 */
		template <typename ...Args>
		struct select_signature
		{
			template < typename TSK >
			static void invoke(TSK& tsk, std::size_t idx, tuple_t const& msg)
			{
				detail::select_signature_impl<sizeof...(Args) - 1, Args...>::invoke(tsk, idx, msg);
			}
		};
		
// 		select_signature<signatures_t>::invoke(impl_, slot, msg);
		template<template<class...> class T, class... Args> 
		struct select_signature<T<Args...>>
		{
			//call_by_index<ML>::invoke(impl_, 0, msg);
			template < typename TSK >
			static void invoke(TSK& tsk, std::size_t idx, tuple_t const& msg)
			{
				detail::select_signature_impl<sizeof...(Args) - 1, Args...>::invoke(tsk, idx, msg);
			}
		};		
	}
}


#endif	//	CYNG_ASYNC_TASK_META_HPP



