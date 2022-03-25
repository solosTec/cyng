/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_VECTOR_CAST_H
#define CYNG_OBJ_VECTOR_CAST_H

#include <cyng/obj/value_cast.hpp>
#include <vector> 
#include <algorithm>
#include <iterator>

#ifdef _DEBUG
#include <cyng/io/ostream.h>
#include <cyng/io/ostream.h>
#endif

#include <boost/assert.hpp>

namespace cyng {

	template < typename T >
	[[nodiscard]]
	std::vector< T > vector_cast(vector_t const& vec, T const& def) noexcept
	{
		std::vector< T > result;
		result.reserve(vec.size());
		std::transform(std::begin(vec), std::end(vec), std::back_inserter(result), [&def](object const& obj) {
			return value_cast<T>(obj, def);
			});
		return result;
	}

	template < typename T >
	[[nodiscard]]
	std::vector< T > vector_cast(tuple_t const& tpl, T const& def) noexcept
	{
		std::vector< T > result;
		result.reserve(tpl.size());
		std::transform(std::begin(tpl), std::end(tpl), std::back_inserter(result), [&def](object const& obj) {
			return value_cast<T>(obj, def);
			});
		return result;
	}

	template < typename T >
	[[nodiscard]]
	std::vector< T > vector_cast(deque_t const& dq, T const& def) noexcept
	{
		std::vector< T > result;
		result.reserve(dq.size());
		std::transform(std::begin(dq), std::end(dq), std::back_inserter(result), [&def](object const& obj) {
			return value_cast<T>(obj, def);
			});
		return result;
	}

	/**
	 * Read a vector of the specified data type.
	 *
	 * example
	 * @code
	  std::vector<std::string> vec = vector_cast<std::string>(obj, "");
	 * @endcode
	 */
	template < typename T >
	[[nodiscard]]
	std::vector< T > vector_cast(object const& obj, T const& def) noexcept
	{
		if (obj.tag() == TC_VECTOR) {
			auto const* vp = object_cast<vector_t>(obj);
			return (vp != nullptr)
				? vector_cast<T>(*vp, def)
				: std::vector< T >{}
			;
		}
		else if (obj.tag() == TC_TUPLE) {
			auto const* tpl = object_cast<tuple_t>(obj);
			return (tpl != nullptr)
				? vector_cast<T>(*tpl, def)
				: std::vector< T >{}
			;
		}
		else if (obj.tag() == TC_DEQUE) {
			auto const* dq = object_cast<deque_t>(obj);
			return (dq != nullptr)
				? vector_cast<T>(*dq, def)
				: std::vector< T >{}
			;
		}
		return std::vector< T >{};
	}
}

#endif //	CYNG_OBJ_VECTOR_CAST_H
