/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_TUPLE_CAST_HPP
#define CYNG_TUPLE_CAST_HPP

#include <cyng/intrinsics/sets.h>
#include <cyng/value_cast.hpp>
#include <cyng/dom/reader.h>
#include <tuple> 

namespace cyng 
{
	namespace detail
	{
		template < std::size_t IDX, std::size_t N, typename T, typename ...Args >
		struct writer
		{
			//typedef typename select_reader<T>::type reader_type;
			typedef typename std::tuple<Args...>	tuple_type;
			typedef typename std::tuple_element<IDX, tuple_type>::type U;

			static void write(T const& inp, tuple_type& out)
			{
				BOOST_ASSERT_MSG(inp.size() > N, "insufficient stack size (read)");
				BOOST_ASSERT_MSG(IDX < inp.size(), "index out of range (read)");

				//	the template keyword signals the compiler that 
				//	a member template name starts here

				//const reader_type reader(inp);
				//const U value = reader.template get<U>(IDX, std::get< IDX >(out));

				//
				//	Rethink this approach. It comes to an error if the tuple contains
				//	an attribute that has the same index as IDX. The real value of the tuple
				//	virtually vanishes, because the attribute index takes precedence.
				//	Use a simple index approach to access tuple members instead.
				//

				auto pos = inp.begin();
				std::advance(pos, IDX);
				const U value = value_cast<U>(*pos, std::get< IDX >(out));

				std::get< IDX >(out) = value;

				//	recursion
				writer<IDX + 1, N - 1, T, Args...>::write(inp, out);

			}
		};

		//	terminate recursion 
		template < std::size_t IDX, typename T, typename ...Args >
		struct writer < IDX, 0, T, Args... >
		{
			typedef typename select_reader<T>::type reader_type;
			typedef typename std::tuple<Args...>	tuple_type;
			typedef typename std::tuple_element<IDX, tuple_type>::type U;

			static void write(T const& inp, std::tuple<Args...>& out)
			{
				BOOST_ASSERT_MSG(inp.size() > 0, "insufficient stack size (read)");
				BOOST_ASSERT_MSG(IDX < inp.size(), "index out of range (read)");

				//const reader_type reader(inp);
				//const U value = reader.template get<U>(IDX, std::get< IDX >(out));
				//std::get< IDX >(out) = value;

				auto pos = inp.begin();
				std::advance(pos, IDX);
				std::get< IDX >(out) = value_cast<U>(*pos, std::get< IDX >(out));

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
			typedef std::integral_constant<std::size_t, sizeof...(Args)>	argc;
			using return_type = typename std::tuple< Args... >	;

			template < typename T >
			using writer_type = writer< 0, argc::value - 1, T, Args...>;

			template< typename T >
			static return_type convert(T const& inp)
			{
				return_type out;
				writer_type< T >::write(inp, out);
				return out;
			}
		};

		/**
		 * specialized for tuples
		 */
		template < typename ... Args >
		struct tuple_cast_helper < std::tuple< Args... >>
		{
			using return_type = typename std::tuple< Args... >;

			template< typename T >
			static return_type convert(T const& inp)
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
			using return_type = std::tuple<>;

			template< typename T >
			static return_type convert(T const& inp)
			{
				return return_type();
			}
		};

	}

	template <typename ... Args>
	auto tuple_cast(tuple_t const& tpl) -> std::tuple<Args...>
	{
		return detail::tuple_cast_helper<Args...>::convert(tpl);
	}

	template <typename ... Args>
	auto tuple_cast(vector_t const& vec) -> std::tuple<Args...>
	{
		return detail::tuple_cast_helper<Args...>::convert(vec);
	}
}

#endif //	CYNG_SET_CAST_HPP

