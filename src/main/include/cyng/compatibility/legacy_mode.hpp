/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_LEGACY_MODE_HPP
#define CYNG_LEGACY_MODE_HPP

#include <CYNG_project_info.h>
#include <cstddef>
#include <utility>

namespace cyng 
{
	namespace meta
	{
		
#if defined(__CPP_SUPPORT_N3658)

		/**
		 * Alias in namespace cyng::meta
		 */
		template<std::size_t... Ints>
		using index_sequence = std::integer_sequence<std::size_t, Ints...>;

		template<std::size_t N>
		using make_index_sequence = std::make_integer_sequence<std::size_t, N>;

#else

		/**
		 * Define our own sequence.
		 *
		 * @code
		 * std::make_index_sequence<N> is
		 * @endcode
		 *
		 * is equal to
		 *
		 * @code
		 * generate_sequence<N>::type or
		 * make_index_sequence<N>
		 * @endcode
		 */
		template<std::size_t ...>
		struct index_sequence
		{ };

		template<std::size_t N, std::size_t ...S>
		struct generate_index_sequence : generate_index_sequence<N - 1, N - 1, S...>
		{ };

		template<std::size_t ...S>
		struct generate_index_sequence<0, S...>
		{
			using type = index_sequence<S...>;
		};

		template<std::size_t ...S>
		using make_index_sequence = typename generate_index_sequence<S...>::type;

#endif
		
	}	
}

#endif // CYNG_LEGACY_MODE_HPP
