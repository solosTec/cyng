/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_INTRINSICS_VERSION_H
#define CYNG_INTRINSICS_VERSION_H


#include <cyng/cyng.h>
#include <utility>
#include <cstdint>
#include <limits>
#include <boost/predef.h>
#if defined(BOOST_OS_LINUX)
#include <sys/types.h>
#undef major
#undef minor
#endif

namespace cyng 
{
	namespace details 
	{
		template <typename T, typename U>
		struct ver : std::pair<T, T>
		{
			using base_type = std::pair<T, T>;
			using this_type = ver<T, U>;
			using full_type = U;
 			using size = std::integral_constant<std::size_t, sizeof(T) * 8>;
			
			static_assert(sizeof(T) * 2 == sizeof(U), "wrong datatype size");
			
			ver(T maj, T min)
			: base_type(maj, min)
			{}
						
			ver(U val)
			: base_type(static_cast<T>(val >> size::value), static_cast<T>(std::numeric_limits<T>::max() & val))
			{}

			T major() const
			{
				return base_type::first;
			}

			T minor() const
			{
				return base_type::second;
			}
			
			U full() const
			{
				return (static_cast<U>(major()) << size::value) + minor();
			}
						
		};
	}
	
	class version : public details::ver<std::uint16_t, std::uint32_t>
	{
	public:
		version(std::uint16_t, std::uint16_t);
		version(std::uint32_t);
		version(version const&);
		version();
	};
	
	class revision : public details::ver<std::uint32_t, std::uint64_t>
	{
	public:
		revision(std::uint32_t, std::uint32_t);
		revision(std::uint64_t);
		revision(revision const&);
		revision();
	};
		
}

#endif	//	CYNG_INTRINSICS_VERSION_H

