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
			switch (obj.rtti().tag()) {
			case TC_BOOL:
				return value_cast(obj, false)
					? std::numeric_limits<T>::lowest()
					: std::numeric_limits<T>::max()
					;

			case TC_CHAR:	return boost::numeric_cast<T>(value_cast<char>(obj, 0));
			case TC_UINT8:	return boost::numeric_cast<T>(value_cast<std::uint8_t>(obj, 0));
			case TC_UINT16:	return boost::numeric_cast<T>(value_cast<std::uint16_t>(obj, 0));
			case TC_UINT32:	return boost::numeric_cast<T>(value_cast<std::uint32_t>(obj, 0));
			case TC_UINT64:	return boost::numeric_cast<T>(value_cast<std::uint64_t>(obj, 0));
			case TC_INT8:	return boost::numeric_cast<T>(value_cast<std::int8_t>(obj, 0));
			case TC_INT16:	return boost::numeric_cast<T>(value_cast<std::int16_t>(obj, 0));
			case TC_INT32:	return boost::numeric_cast<T>(value_cast<std::int32_t>(obj, 0));
			case TC_INT64:	return boost::numeric_cast<T>(value_cast<std::int64_t>(obj, 0));

			case TC_FLOAT:		return static_cast<T>(value_cast<float>(obj, 0.0));
			case TC_DOUBLE:		return static_cast<T>(value_cast<double>(obj, 0.0));
			case TC_FLOAT80:	return static_cast<T>(value_cast<long double>(obj, 0.0));
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
#endif
		}
		catch (boost::numeric::negative_overflow const& ex) {
#ifdef _DEBUG_OBJECT
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
#endif
		}
		catch (boost::numeric::bad_numeric_cast const& ex) {
#ifdef _DEBUG_OBJECT
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
#endif
		}
		catch (std::invalid_argument const& ex) {
#ifdef _DEBUG_OBJECT
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
#endif
		}
		return def;
	}
}

#endif //	CYNG_OBJ_NUMERIC_CAST_H
