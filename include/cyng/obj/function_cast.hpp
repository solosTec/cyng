/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_FUNCTION_CAST_H
#define DOCC_OBJ_FUNCTION_CAST_H

#include <cyng/obj/tuple_cast.hpp>
#include <tuple> 
#include <utility> 
#include <functional>

#include <boost/assert.hpp>

namespace docscript {

	//template<typename... Args>
	//struct identity { };

	namespace {


		template<typename Function>
		struct unwrap_function_impl;

		template<typename R, typename... Args>
		struct unwrap_function_impl<R(Args...)>
		{
			/// The return type of the function.
			using return_t = R;
			using tuple_t = std::tuple<Args...>;

			/// The argument types of the function as pack in identity.
			//typedef identity<Args...> argument_type;

			using size = std::integral_constant<std::size_t, sizeof...(Args)>;

			/// The function provided as std::function
			using function_t = std::function<R(Args...)> ;

			/// The function provided as function_ptr
			typedef R(*function_ptr)(Args...);
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

	/**
	 * Helps to convert a type list into a function call
	 */
	template <typename... F>
	struct f_type
	{
		using function_t = typename unwrap_function_impl<F...>::function_t;
		using return_t = typename unwrap_function_impl<F...>::return_t;
		using tuple_t = typename unwrap_function_impl<F...>::tuple_t;
	};

	template <typename... F>
	[[nodiscard]]
	auto function_cast(tuple_t const& tpl)-> typename unwrap_function_impl<F...>::tuple_t
	{
		using u = unwrap_function_impl<F...>;
		using tuple_type = typename u::tuple_t; 

		//std::cout << u::size::value << ", " << std::tuple_size< tuple_t>::value << std::endl;

		return tuple_cast<tuple_type>(tpl);
	}

	template <typename... F>
	auto function_call(typename unwrap_function_impl<F...>::function_t f, tuple_t const& tpl)-> typename unwrap_function_impl<F...>::return_t
	{
		using u = unwrap_function_impl<F...>;
		using tuple_t = typename u::tuple_t;
		using R = typename u::return_t;

//#ifdef _DEBUG
//		auto const tmp = tuple_cast<tuple_t>(tpl);
//#endif

		return std::apply(f, tuple_cast<tuple_t>(tpl));
	}

}

#endif //	DOCC_OBJ_FUNCTION_CAST_H
