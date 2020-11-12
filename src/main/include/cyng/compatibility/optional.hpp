/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Sylko Olzscher
 *
 */
#ifndef CYNG_OPTIONAL_HPP
#define CYNG_OPTIONAL_HPP

#if defined(__CPP_SUPPORT_P0220R1)
#include <optional>
#else
#include <boost/optional.hpp>
#endif
#include <cstddef>
#include <utility>

namespace cyng
{
	//
	//	Bring in a compatible version of optional into cyng namespace
	//
	//	Note: Boost.Optional doesn't support the nullopt_t type (nullopt) and uses 
	//	boost::none_t (none) instead.
	//
	//	There are more differences whne using references!
	//

#if defined(__CPP_SUPPORT_P0220R1)

	template < typename T >
	using optional = std::optional< T >;

	constexpr auto nullopt = std::nullopt;

#else

	template < typename T >
	using optional = boost::optional< T >;

	constexpr auto nullopt = boost::none_t;

#endif

	/**
	 * Convert a pair<T,bool> depending on the bool value.
	 */
	template < typename T >
	optional<T> to_optional(std::pair<T, bool>&& p) {
		return (p.second)
			? optional<T>(std::forward<T>(p.first))
			: optional<T>{}
		;
	}

}

#endif 
