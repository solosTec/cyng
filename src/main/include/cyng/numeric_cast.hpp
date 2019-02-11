/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_NUMERIC_CAST_HPP
#define CYNG_NUMERIC_CAST_HPP

#include <type_traits>
#include <limits.h>
#include <cyng/value_cast.hpp>
#include <boost/numeric/conversion/cast.hpp>
 
namespace cyng 
{
    template < typename T >
    T string_to_numeric(object const& obj, T const& def) noexcept
    {
        static_assert(std::is_arithmetic<T>::value, "only arithmetic types supported");
        try {
            const std::string defll = std::to_string(def);
            const std::string inp = cyng::value_cast<std::string>(obj, defll);
            return (std::is_unsigned<T>::value)
                ? std::stoull(inp)
                : std::stoll(inp)
                ;
        }
        catch (std::exception const&) {
        }
        return def;
    }

	/**
	 * If the hidden value is not of type T the function returns the default value.
	 *
	 * @tparam T cast object 
	 * @param def default value
	 * @return a const reference const& of the value or the default value if 
	 */
	template < typename T >
	T numeric_cast(object const& obj, T const& def) noexcept
	{
		static_assert(std::is_arithmetic<T>::value, "only arithmetic types supported");
		try {
			switch (obj.get_class().tag())	{
				
				case TC_NULL:	return static_cast<T>(0);
				
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
		catch (boost::numeric::positive_overflow const& ex)	{
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
		}
		catch (boost::numeric::negative_overflow const& ex)	{
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
		}
		catch (boost::numeric::bad_numeric_cast const& ex)	{
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
		}
		catch (std::invalid_argument const& ex)	{
			std::cerr
				<< "***Warning: "
				<< ex.what()
				<< std::endl
				;
		}
		return def;
	}	
}

#endif //	CYNG_NUMERIC_CAST_HPP
