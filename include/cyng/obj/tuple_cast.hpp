/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_TUPLE_CAST_HPP
#define CYNG_OBJ_TUPLE_CAST_HPP

#include <cyng/obj/value_cast.hpp>
#include <tuple> 
#include <utility> 
#include <functional>

#ifdef _DEBUG_OBJECT
#include <cyng/io/ostream.h>
#include <iostream>
#endif

#include <boost/assert.hpp>

namespace cyng {

	namespace {

		template < std::size_t N, typename T, typename ...Args >
		struct convert
		{
			using tuple_t = typename std::tuple< std::decay_t<Args> ...>;
			using U = typename std::tuple_element<N, tuple_t>::type;
			using iter_t = typename T::const_reverse_iterator;

			static void set(iter_t pos, iter_t end, tuple_t& out)
			{
#ifdef _DEBUG_OBJECT
				std::cerr << "N: " << N << " - " << *pos << std::endl;
#endif
				if (pos != end) {
					std::get< N >(out) = value_cast<U>(*pos, std::get< N >(out));
					//	recursion
					convert<N - 1, T, Args...>::set(++pos, end, out);
				}
			}
		};

		//	terminate recursion 
		template < typename T, typename ...Args >
		struct convert < 0, T, Args... >
		{
			using tuple_t = typename std::tuple< std::decay_t<Args> ...>;

			using U = typename std::tuple_element<0, tuple_t>::type;
			using iter_t = typename T::const_reverse_iterator;

			static void set(iter_t pos, iter_t end, std::tuple<Args...>& out)
			{
#ifdef _DEBUG_OBJECT
				std::cerr << "N: 0 - " << *pos  << std::endl;
#endif
				BOOST_ASSERT(pos != end);
				std::get< 0 >(out) = value_cast<U>(*pos, std::get< 0 >(out));

			}
		};

		/**
		 * Convert a data set into a type safe std::tuple.
		 * Since we cannot stop recursion depended from the
		 * tuple size we have to introduce a second parameter IDX.
		 * This allows not to read in reverse order starting
		 * with 0.
		 */
		template < typename ... Args >
		struct tuple_cast_helper
		{
			using size = std::integral_constant<std::size_t, sizeof...(Args)>;
			using R = typename std::tuple< Args... >;

			template < typename T >
			using cast_type = convert< size::value - 1, T, Args...>;

			template< typename T >
			static R convert(T const& inp)
			{
				//	compile time meets runtime
				using size = std::integral_constant<std::size_t, sizeof...(Args)>;
				BOOST_ASSERT(size::value == inp.size());

				R out;
				cast_type< T >::set(inp.crbegin(), inp.crend(), out);
				return out;
			}
		};

		template <typename ... Args>
		struct tuple_cast_helper< std::tuple< Args... >>
		{
			using R = typename std::tuple< std::decay_t<Args> ...>;

			template< typename T >
			static R convert(T const& inp)
			{
				return /*typename*/ tuple_cast_helper< Args...>::template convert< T >(inp);
			}
		};

		 /**
		  * accept empty tuples
		  */
		template <>
		struct tuple_cast_helper <std::tuple<>>
		{
			using R = typename std::tuple<>;

			template< typename T >
			static R convert(T const&)
			{
				return R();
			}
		};
	}


	/**
	 * Takes a list of objects and cast all elements into the specified tuple class.
	 * 
	 * Example:
	 * @code
	auto tpl = tuple_factory(1, "hello", 2.3f);
	auto const [i,s,f] = tuple_cast<int, std::string, float>(tpl);
	 * @endcode
	 */
	template <typename ... Args>
	auto tuple_cast(tuple_t const& tpl)-> typename tuple_cast_helper<Args...>::R
	{
		using R = typename tuple_cast_helper<Args...>::R;

		//	compile time meets runtime
		//constexpr auto size = sizeof...(Args);
		//BOOST_ASSERT(size >= tpl.size());
		return tuple_cast_helper<std::decay_t<Args>...>::convert(tpl);
		//return (size == tpl.size())
		//	? tuple_cast_helper<std::decay_t<Args>...>::convert(tpl)
		//	: R{}
		//	;
	}

	/**
	 * Takes a vector of objects and cast all elements into the specified tuple class.
	 */
	template <typename ... Args>
	auto tuple_cast(vector_t const& vec) -> std::tuple< std::decay_t<Args> ...>
	{
		using R = typename std::tuple< std::decay_t<Args> ...>;

		//	compile time meets runtime
		constexpr auto size = sizeof...(Args);
		BOOST_ASSERT(size == vec.size());

		return (size == vec.size())
			//	works for vectors too
			? tuple_cast_helper<std::decay_t<Args>...>::convert(vec)
			: R{}
			;
	}


	template <typename ... Args>
	auto tuple_cast(deque_t const& deq) -> std::tuple< std::decay_t<Args> ...>
	{
		using R = typename std::tuple< std::decay_t<Args> ...>;

		//	compile time meets runtime
		constexpr auto size = sizeof...(Args);
		BOOST_ASSERT(size == deq.size());

		return (size == deq.size())
			//	works for deques too
			? tuple_cast_helper<std::decay_t<Args>...>::convert(deq)
			: R{}
		;
	}

	/**
	 * Parsing a buffer into a std::tuple<...> utilizing the functions defined in buffer_cast.hpp.
	 */
	//	ToDo: implement a generic buffer -> std::tuple<> cast
	//template <typename ... Args>
	//auto tuple_cast(cyng::buffer_t const& data) -> std::tuple< std::decay_t<Args> ...> {

	//	using R = typename std::tuple< std::decay_t<Args> ...>;
	//	constexpr std::size_t upper_bound = sizeof...(Args) - 1;
	//	R r{};
	//	buffer_to_tuple_cast_helper<upper_bound, std::decay_t<Args> ...>::cast(data, r, 0);
	//	return r;
	//}

}

#endif //	CYNG_OBJ_TUPLE_CAST_HPP
