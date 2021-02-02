/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_TUPLE_CAST_H
#define DOCC_OBJ_TUPLE_CAST_H

#include <cyng/obj/value_cast.hpp>
#include <tuple> 
#include <utility> 
#include <functional>

#ifdef _DEBUG_OBJECT
#include <cyng/io/ostream.h>
#include <cyng/io/ostream.h>
#endif

#include <boost/assert.hpp>

namespace docscript {

	namespace {

		template < std::size_t N, typename T, typename ...Args >
		struct convert
		{
			using tuple_t = typename std::tuple<Args...>;
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
			using tuple_t = typename std::tuple<Args...>;
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
			using R = typename std::tuple< Args... >;

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
			static R convert(T const& inp)
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
		//	compile time meets runtime
		using size = std::integral_constant<std::size_t, sizeof...(Args)>;
		BOOST_ASSERT(size::value == 1);

		return tuple_cast_helper<Args...>::convert(tpl);
	}

	/**
	 * Takes a vector of objects and cast all elements into the specified tuple class.
	 */
	template <typename ... Args>
	auto tuple_cast(vector_t const& vec) -> std::tuple<Args...>
	{
		//	compile time meets runtime
		using size = std::integral_constant<std::size_t, sizeof...(Args)>;
		BOOST_ASSERT(size::value == vec.size());

		return tuple_cast_helper<Args...>::convert(vec);
	}

}

#endif //	DOCC_OBJ_TUPLE_CAST_H
