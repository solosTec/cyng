/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_FUNCTION_CAST_H
#define CYNG_OBJ_FUNCTION_CAST_H

#include <cyng/obj/tuple_cast.hpp>
#include <tuple> 
#include <utility> 
#include <functional>

#include <boost/assert.hpp>

namespace cyng {

	namespace {


		template<typename Function>
		struct unwrap_function_impl;

		template<typename R, typename... Args>
		struct unwrap_function_impl<R(Args...)>
		{
			/// The return type of the function.
			using return_t = R;
			//using tuple_t = std::tuple<Args...>;
			using arg_t = std::tuple<std::decay_t<Args>...>;

			/// The argument types of the function as pack in identity.
			//typedef identity<Args...> argument_type;

			using size = std::integral_constant<std::size_t, sizeof...(Args)>;

			/// The function provided as std::function
			using function_t = std::function<R(Args...)> ;

			/// The function provided as function_ptr
			typedef R(*function_ptr)(Args...);
			
			static arg_t cast(tuple_t const& tpl) {
				return tuple_cast<Args...>(tpl);
			}

		};

		/// STL: std::function
		template<typename R, typename... Args>
		struct unwrap_function_impl<std::function<R(Args...)>>
			: unwrap_function_impl<R(Args...)> 
		{ };

		/// STL: std::tuple
		template<typename R, typename... Args>
		struct unwrap_function_impl<std::tuple<R, Args...>>
			: unwrap_function_impl<R(Args...)> 
		{ };

		/// Function pointers
		template<typename T, typename... Args>
		struct unwrap_function_impl<T(* const)(Args...)>
			: unwrap_function_impl<T(Args...)> 
		{ };

		/// Class Method pointers
		template<typename T, typename R, typename... Args>
		struct unwrap_function_impl<R(T::*)(Args...) const>
			: unwrap_function_impl<R(Args...)> 
		{ };

		template<typename T, typename R, typename... Args>
		struct unwrap_function_impl<R(T::*)(Args...)>
			: unwrap_function_impl<R(Args...)>
		{ };

	}


	template <typename... F>
	[[nodiscard]]
	auto function_cast(tuple_t const& tpl)-> typename unwrap_function_impl<F...>::arg_t
	{
		using u = unwrap_function_impl<F...>;
		using tuple_type = typename u::arg_t;

		constexpr auto size = std::tuple_size< tuple_type >::value;
#ifdef __DEBUG
		std::cout << tpl.size() << ", " << size << std::endl;
#endif
		//
		// call tuple_cast<>() with expanded tuple_type
		// 
		return u::cast(tpl);
	}

	template <typename... F>
	auto function_call(typename unwrap_function_impl<F...>::function_t f, tuple_t const& tpl)-> typename unwrap_function_impl<F...>::return_t
	{
		using u = unwrap_function_impl<F...>;
		using tuple_t = typename u::arg_t;

		return std::apply(f, tuple_cast<tuple_t>(tpl));
	}

}

#endif //	CYNG_OBJ_FUNCTION_CAST_H
