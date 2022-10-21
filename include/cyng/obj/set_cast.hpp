/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_SET_CAST_H
#define CYNG_OBJ_SET_CAST_H

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
	std::set< T > set_cast(deque_t const& dq, T const& def) noexcept {
		std::set< T > result;
		std::transform(dq.begin(), dq.end(), std::inserter(result, result.end()), [&def](object const& obj) {
			return value_cast<T>(obj, def);
			});
		return result;
	}
	template < typename T >
	[[nodiscard]]
	std::set< T > set_cast(vector_t const& vec, T const& def) noexcept {
		std::set< T > result;
		std::transform(vec.begin(), vec.end(), std::inserter(result, result.end()), [&def](object const& obj) {
			return value_cast<T>(obj, def);
			});
		return result;
	}
	template < typename T >
	[[nodiscard]]
	std::set< T > set_cast(tuple_t const& tpl, T const& def) noexcept {
		std::set< T > result;
		std::transform(tpl.begin(), tpl.end(), std::inserter(result, result.end()), [&def](object const& obj) {
			return value_cast<T>(obj, def);
			});
		return result;
	}

	/**
	 * Read a vector of the specified data type.
	 *
	 * example
	 * @code
	  std::set<std::string> set = set_cast<std::string>(obj, "");
	 * @endcode
	 */
	template < typename T >
	[[nodiscard]]
	std::set< T > set_cast(object const& obj, T const& def) noexcept
	{
		if (obj.tag() == TC_DEQUE) {
			auto const* dq = object_cast<deque_t>(obj);
			return (dq != nullptr)
				? set_cast<T>(*dq, def)
				: std::set< T >{}
			;
		}
		else if (obj.tag() == TC_VECTOR) {
			auto const* vp = object_cast<vector_t>(obj);
			return (vp != nullptr)
				? set_cast<T>(*vp, def)
				: std::set< T >{}
			;
		}
		else if (obj.tag() == TC_TUPLE) {
			auto const* tpl = object_cast<tuple_t>(obj);
			return (tpl != nullptr)
				? set_cast<T>(*tpl, def)
				: std::set< T >{}
			;
		}
		return std::set< T >{};
	}
}

#endif //	CYNG_OBJ_VECTOR_CAST_H
