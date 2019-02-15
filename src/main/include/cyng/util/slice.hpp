/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_UTIL_SLICE_H
#define CYNG_UTIL_SLICE_H

#include <array>
#include <vector>
#include <stdexcept>

namespace cyng 
{
	/**
	 * The evil hack is to cast a pointer back to an array.
	 * 
	 * @tparam FROM start index
	 * @tparam TO index one behind the last element
	 * @tparam T value type of array
	 * @tparam DIM size of original index
	 * @return a reference to the sliced array
	 */
	template < std::size_t FROM, std::size_t TO, typename T, std::size_t DIM >
	std::array< T, TO - FROM>& slice( std::array< T, DIM >& a )
	{
		static_assert(TO <= DIM && FROM < DIM, "index out of range" );

		typedef std::array< T, TO - FROM >	result_type;
		return *reinterpret_cast< result_type* >(&a[ FROM ]);
	}
	
	/**
	 * @tparam FROM start index
	 * @tparam TO index one behind the last element
	 * @tparam T value type of array
	 * @tparam DIM size of original index
	 * @return a const reference to the sliced array
	 * 
	 * Example:
	 * @code
	 * typedef std::array< std::uint8_t, 6 >	address_type;
	 * typedef std::array< std::uint8_t, 3 >	oui_type, nic_type;
	 * 
	 * const address_type a;
	 * oui_type const& oui = slice<0, 3>(a);
	 * nic_type const& nic = slice<3, 6>(a);
	 * @endcode
	 */
	template < std::size_t FROM, std::size_t TO, typename T, std::size_t DIM >
	std::array< T, TO - FROM> const& slice( std::array< T, DIM > const& a )
	{
		static_assert(TO <= DIM && FROM < DIM, "index out of range" );

		typedef std::array< T, TO - FROM >	result_type;
		return *reinterpret_cast< result_type const* >(&a[ FROM ]);
	}

	/**
	 * @tparam LENGTH of target array
	 * @tparam T value type of array
	 * @tparam DIM size of original index
	 * @param from start index
	 * @return a reference to the sliced array
	 */
	template < std::size_t LENGTH, typename T, std::size_t DIM >
	std::array< T, LENGTH>& slice( std::array< T, DIM >& a, std::size_t from )
	{
		if (from + LENGTH >= DIM)	throw std::range_error("slice array");
		
		typedef std::array< T, LENGTH >	result_type;
		return *reinterpret_cast< result_type* >(&a[ from ]);
	}

	/**
	 * @tparam LENGTH of target array
	 * @tparam T value type of array
	 * @tparam DIM size of original index
	 * @param from start index
	 * @return a const reference to the sliced array
	 */
	template < std::size_t LENGTH, typename T, std::size_t DIM >
	std::array< T, LENGTH> const& slice( std::array< T, DIM > const& a, std::size_t from )
	{
		if (from + LENGTH >= DIM)	throw std::range_error("slice array");
		
		typedef std::array< T, LENGTH >	result_type;
		return *reinterpret_cast< result_type const* >(&a[ from ]);
	}
	
	/**
	 * Slice in integral types
	 */
	template < typename R, std::size_t FROM, typename T, std::size_t DIM >
	R& slicer(std::array< T, DIM >& a)
	{
		static_assert(FROM + sizeof(R) <= DIM, "index out of range");

		//typedef std::array< T, TO - FROM >	result_type;
		return *reinterpret_cast< R* >(&a[FROM]);
	}

	/**
	 * Slice a vector by copying elements into a new vector. In this means it's not a real
	 * slicing. Since working with copies the original data are not affected. But it's
	 * quit usefull when working with cyng objects. In this case only pointers are copied
	 * so the sliced vector enables acces to original data.
	 */
	template < typename T >
	std::vector< T > slice(typename std::vector< T >::iterator first, typename std::vector< T >::iterator last)
	{
		return std::vector< T >(first, last);
	}
	
	/**
	 * This is the version for const interators.
	 */
	template < typename T >
	std::vector< T > slice(typename std::vector< T >::const_iterator first, typename std::vector< T >::const_iterator last)
	{
		return std::vector< T >(first, last);
	}

	/**
	 * Convert an interal type into an array of smaller integral types
	 */
	template <typename U, typename T>
	auto to_array(T n) -> std::array< U, sizeof(T) / sizeof(U) >
	{
		static_assert(sizeof(T) > sizeof(U), "subtype must be smaller");
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");

		typedef std::array< U, sizeof(T) / sizeof(U) >	result_type;

		auto const begin = reinterpret_cast<U const*>(&n);
		auto const end = begin + sizeof(T);

		result_type a;
        
        //  requires C++17
// 		std::reverse_copy(begin, end, a.begin());
        
        auto dest = a.begin();
        while (begin != end) {
            *(dest++) = *(--end);
        }

		return a;
	}

	/**
	 * Convert an interal type into a vector of smaller integral types
	 */
	template <typename U, typename T>
	auto to_vector(T n) -> std::vector< U >
	{
		static_assert(sizeof(T) > sizeof(U), "subtype must be smaller");
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");

		typedef std::vector< U >	result_type;

		auto const begin = reinterpret_cast<U const*>(&n);
		auto const end = begin + sizeof(T);

		//result_type v(begin, end);
		//std::reverse(v.begin(), v.end());
		//return v;

		return result_type(begin, end);
	}

}

#endif 	// CYNG_UTIL_SLICE_H

