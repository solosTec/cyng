/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_NUMERIC_CAST_H
#define CYNG_OBJ_NUMERIC_CAST_H

#include <cyng/obj/value_cast.hpp>
#include <cyng/obj/core/type.hpp>

#include <type_traits>
#include <sstream>

#ifdef _DEBUG_OBJECT
#include <iostream>
#endif

#include <boost/numeric/conversion/cast.hpp>
#include<boost/core/ignore_unused.hpp>

namespace cyng {

	template < typename T >
	[[nodiscard]]
	T string_to_numeric(object const& obj, T const& def) noexcept
	{
		const std::string defs = std::to_string(def);

		std::stringstream ss;
		T r = T();
		ss << value_cast<std::string>(obj, defs);
		ss >> r;

		return r;
	}

	/**
	 * If the hidden value is not of type T the function returns the default value.
	 *
	 * @tparam T cast object
	 * @param def default value
	 * @return a const reference const& of the value or the default value if
	 */
	template < typename T >
	[[nodiscard]]
	T numeric_cast(object const& obj, T const& def) noexcept
	{
		static_assert(std::is_arithmetic<T>::value, "only arithmetic types supported");
		try {
			switch (obj.tag()) {
			case TC_BOOL: {
				auto const b = value_cast(obj, false);
				//	if expected and provided type both booleans don't return a numeric limit
				if constexpr (std::is_same_v<T, bool>) {
					return b;
				}
				return b
					? std::numeric_limits<T>::lowest()
					: std::numeric_limits<T>::max()
					;
			}

			case TC_CHAR:	return boost::numeric_cast<T>(value_cast<char>(obj, 0));
			case TC_UINT8:	return boost::numeric_cast<T>(value_cast<std::uint8_t>(obj, 0));
			case TC_UINT16: {
				auto const n = value_cast<std::uint16_t>(obj, 0);
				if constexpr (std::is_same_v<T, std::uint16_t>) {
					return n;
				}
				return boost::numeric_cast<T>(n);
			}
			case TC_UINT32: {
				auto const n = value_cast<std::uint32_t>(obj, 0);
				if constexpr (std::is_same_v<T, std::uint32_t>) {
					return n;
				}
				return boost::numeric_cast<T>(n);
			}
			case TC_UINT64: {
				auto const n = value_cast<std::uint64_t>(obj, 0);
				if constexpr (std::is_same_v<T, std::uint64_t>) {
					return n;
				}
				return boost::numeric_cast<T>(n);
			}
			case TC_INT8:	return boost::numeric_cast<T>(value_cast<std::int8_t>(obj, 0));
			case TC_INT16: {
				auto const n = value_cast<std::int16_t>(obj, 0);
				if constexpr (std::is_same_v<T, std::int16_t>) {
					return n;
				}
				return boost::numeric_cast<T>(n);
			}
			case TC_INT32: {
				auto const n = value_cast<std::int32_t>(obj, 0);
				if constexpr (std::is_same_v<T, std::int32_t>) {
					return n;
				}
				return boost::numeric_cast<T>(n);
			}
			case TC_INT64:	{
				auto const n = value_cast<std::int64_t>(obj, 0);
				if constexpr (std::is_same_v<T, std::int64_t>) {
					return n;
				}
				return boost::numeric_cast<T>(n);
			}

			case TC_FLOAT: {
				auto const f = value_cast<float>(obj, 0.0);
				if constexpr (std::is_same_v<T, float>) {
					return f;
				}
				return static_cast<T>(f);
			}
			case TC_DOUBLE: {
				auto const d = value_cast<double>(obj, 0.0);
				if constexpr (std::is_same_v<T, double>) {
					return d;
				}
				return static_cast<T>(d);
			}
			case TC_FLOAT80: {
				auto const d = value_cast<long double>(obj, 0.0);
				if constexpr (std::is_same_v<T, long double>) {
					return d;
				}
				return static_cast<T>(d);
			}
			case TC_STRING:		return string_to_numeric<T>(obj, def);
			default:
				break;
			}
		}
		catch (boost::numeric::positive_overflow const& ex) {
#ifdef _DEBUG_OBJECT
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
#else
			boost::ignore_unused(ex);
#endif
		}
		catch (boost::numeric::negative_overflow const& ex) {
#ifdef _DEBUG_OBJECT
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
#else
			boost::ignore_unused(ex);
#endif
		}
		catch (boost::numeric::bad_numeric_cast const& ex) {
#ifdef _DEBUG_OBJECT
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
#else
			boost::ignore_unused(ex);
#endif
		}
		catch (std::invalid_argument const& ex) {
#ifdef _DEBUG_OBJECT
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
#else
			boost::ignore_unused(ex);
#endif
		}
		return def;
	}
}

#endif //	CYNG_OBJ_NUMERIC_CAST_H
