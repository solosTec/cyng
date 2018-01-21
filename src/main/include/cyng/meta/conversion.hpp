/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_META_CONVERSION_HPP
#define CYNG_META_CONVERSION_HPP

#include <cyng/meta/meta.hpp>
#include <cyng/value_cast.hpp>
#include <iostream>
#include <boost/uuid/uuid_io.hpp>

namespace cyng 
{
	namespace meta 
	{
		
		//	generalized approach
		namespace detail
		{
			template < std::size_t IDX, std::size_t N, typename I, typename ...Args >
			struct converter_impl
			{
				using tuple_type = typename std::tuple<Args...>	;
				using value_type = typename std::tuple_element<IDX, tuple_type>::type;

				static void copy(I pos, I end, tuple_type& out)
				{
					if (pos != end)
					{
						//std::cout << value_cast<value_type>(*pos, std::get< IDX >(out)) << std::endl;
						//
						//	assign converted value (copy)
						//
						std::get< IDX >(out) = value_cast<value_type>(*pos, std::get< IDX >(out));

						//	recursion
						converter_impl<IDX + 1, N - 1, I, Args...>::copy(++pos, end, out);
					}
					else
					{
						//
						//	set default value
						//
						std::get< IDX >(out) = value_type();
					}
				}
			};

			//	terminate recursion 
			template < std::size_t IDX, typename I, typename ...Args >
			struct converter_impl < IDX, 0, I, Args... >
			{
				using tuple_type = typename std::tuple<Args...>	;
				using value_type = typename std::tuple_element<IDX, tuple_type>::type;

				static void copy(I pos, I end, std::tuple<Args...>& out)
				{
					//std::cout << value_cast<value_type>(*pos, std::get< IDX >(out)) << std::endl;
					std::get< IDX >(out) = value_cast<value_type>(*pos, std::get< IDX >(out));
				}
			};

		}	// detail

		/**
		 * Convert a data set into a type safe std::tuple.
		 * Since we cannot stop recursion depended from the
		 * tuple size we have to introduce a second parameter IDX. 
		 * This allows not to read in reverse order starting
		 * with 0.
		 */
		template < typename ... Args >
		struct converter_factory
		{
			using argc = std::integral_constant<std::size_t, sizeof...(Args)>;
			using return_type = typename std::tuple< Args... >;

			template < typename I >
			using converter_type = detail::converter_impl< 0, argc::value - 1, I, Args...>;

			template< typename I >
			static return_type copy(I begin, I end)
			{
				return_type out;
				converter_type< I >::copy(begin, end, out);
				return out;
			}
		};

		/**
		 * specialized for tuples
		 */
		template < typename ... Args >
		struct converter_factory < std::tuple< Args... >>
		{
			typedef typename std::tuple< Args... >	return_type;

			template< typename I >
			static return_type copy(I begin, I end)
			{
				return converter_factory< Args...>::template copy< I >(begin, end);
			}
		};

		/**
		 * Specialized for empty tuples
		 */
		template <>
		struct converter_factory <std::tuple<>>
		{
			typedef std::tuple<>	return_type;
			template< typename I >
			static return_type copy(I begin, I end)
			{
				return return_type();
			}
		};	
	}
}


#endif	//	CYNG_META_CONVERSION_HPP



