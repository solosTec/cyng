/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_META_HPP
#define CYNG_META_HPP

#include <CYNG_project_info.h>
#include <cyng/compatibility/legacy_mode.hpp>
#include <type_traits>
#include <tuple>

#if defined(CYNG_STD_APPLY_OFF)
#include <boost/fusion/functional/invocation/invoke_function_object.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/adapted/std_tuple.hpp>
#include <boost/fusion/include/std_tuple.hpp>
#endif

namespace cyng 
{
	namespace meta 
	{
		/**
		 * Metafunction invocation.
		 * Assuming type is the return type/value of a meta function.
		 */
		template <typename T>
		using invoke = typename T::type;

		/**
		 * Identity metafunction
		 */
		template <typename T>
		struct identity 
		{
			using type = T;
		};
		
		/**
		 * @see http://pdimov.com/cpp2/simple_cxx11_metaprogramming.html
		 * @see http://www.boost.org/doc/libs/master/libs/mp11/doc/html/mp11.html
		 */
		namespace detail 
		{
			template<template<class...> class F, class L> 
			struct transform_impl;
			
			template<template<class...> class F, template<class...> class L, class... T>
			struct transform_impl<F, L<T...>>
			{
				//
				//	F<T>... transforms the parameter list
				//
				using type = L<F<T>...>;
			};
		}
		
		/**
		 * Example:
		 * @code
		 * using t = meta::transform<std::add_pointer, std::pair<int, float>>;
		 * @endcode
		 */
		template<template<class...> class F, class L>
		using transform = typename detail::transform_impl<F, L>::type;

		
	
//	BOOST_NO_CXX17_STD_APPLY doesn't fit
#if defined(CYNG_STD_APPLY_OFF)
		
		//
		//	Leveraging Boost.Fusion
		//	Note: There is a restriction to max. 5 parameters in the sequence when using this boost version.
		//
		template <class F, class Sequence>
		constexpr auto apply(F f, Sequence & s) -> typename  boost::fusion::result_of::invoke_function_object<F,Sequence>::type
		{
			return boost::fusion::invoke_function_object<F, Sequence>(f, s);
		}
		
		template <class F, class Sequence>
		constexpr auto apply(F f, Sequence const & s) -> typename  boost::fusion::result_of::invoke_function_object<F,Sequence const>::type
		{
			return boost::fusion::invoke_function_object<F, Sequence const>(f, s);
		}
		
		//
		//	This is an attempt to write a std::apply() function that works
		//	with gcc 4.7. One requirement is that F is a function object.
		//	Otherwise the detection of the return type doesn't work.
		//
		
		namespace detail 
		{
			template <class F, class Tuple, std::size_t... I>
			constexpr auto cyng_apply_impl(F f, Tuple&& tpl, index_sequence<I...>) -> typename F::result_type
			{
				//	call function with expanded unpacked tuple arguments
 				return f(std::get<I>(std::forward<Tuple>(tpl))...);
			}
		}  // namespace detail
		
		/**
		 * Invoke the Callable object f with a tuple of arguments.
		 */
		template <class F, class Tuple>
		constexpr auto cyng_apply(F f, Tuple&& tpl) -> typename F::result_type
		{
			return detail::cyng_apply_impl(
				f, 
				std::forward<Tuple>(tpl),
				make_index_sequence<std::tuple_size<Tuple>::value>{});
		}
		
#else
		template <class F, class Sequence>
		constexpr decltype(auto) 
		apply(F&& f, Sequence&& s)
		{
			return std::apply(std::forward<F>(f), std::forward<Sequence>(s));
		}

		/**
		 * 
		 * Visit a specific element.
		 * 
		 */
		template <std::size_t N>
		struct visit_at_impl
		{
			template <typename F, typename... Args>
			static 
			constexpr decltype(auto) 
			visit(F&& f, std::size_t idx, Args&&...  args)
			{
// 				std::cout << "visit-at: " << idx << ", " << N << std::endl;
				return (idx == N)
				? f(std::forward<Args>(args)...)
				: visit_at_impl<N-1>::visit(std::forward<F>(f), idx, std::forward<Args>(args)...);
			}
		};
		
		template <>
		struct visit_at_impl< 0 >
		{
			template <typename F, typename... Args>
			static 
			constexpr decltype(auto) 
			visit(F&& f, std::size_t idx, Args&&...  args)
			{
// 				std::cout << "visit-at: " << idx << ", NULL" << std::endl;
				return f(std::forward<Args>(args)...);
			}
		};

		/**
		 * Visit a specific element.
		 */
		template <class F, typename... Args>
		constexpr decltype(auto) 
		visit_at(F&& f, std::size_t idx, Args&&...  args)
		{
			return visit_at_impl<sizeof...(Args)>::visit(std::forward<F>(f), idx, std::forward<Args>(args)...);
		}

		template <class F, typename Tpl, std::size_t... M>
		constexpr decltype(auto) 
		visit_at(F&& f, std::size_t idx, Tpl&& tpl, index_sequence<M...>)
		{
			return visit_at(std::forward<F>(f), idx, std::get<M>(std::forward<Tpl>(tpl))...);
		}
		
		template <class F, typename... Args>
		constexpr decltype(auto) 
		visit_at(F&& f, std::size_t idx, std::tuple<Args...>&& tpl)
		{
			using tuple_t = std::tuple<Args...>;
			using seq = std::make_index_sequence < sizeof... (Args)>;
			return visit_at(std::forward<F>(f), idx, std::forward<tuple_t>(tpl), seq{});
		}
#endif

	}	//	meta
}

#endif	//	CYNG_META_HPP


