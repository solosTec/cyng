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
#include <sstream>
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
			
			constexpr ver(T maj, T min)
			: base_type(maj, min)
			{}
						
			constexpr ver(U val)
			: base_type(static_cast<T>(val >> size::value), static_cast<T>(std::numeric_limits<T>::max() & val))
			{}

			constexpr T major() const
			{
				return base_type::first;
			}

			constexpr T minor() const
			{
				return base_type::second;
			}
			
			constexpr U full() const
			{
				return (static_cast<U>(major()) << size::value) + minor();
			}

			/**
			 * major element has a higher precedience.
			 */
			constexpr bool is_less(this_type const& other) const
			{
				return (this->major() == other.major())
					? (this->minor() < other.minor())
					: (this->major() < other.major())
					;
			}

			/**
			 * Convert a version into a representation as double value in the form
			 * of major.minor. Doesn't work well for some combinations e.g. 
			 * 0 + (7/10) is 0.69999999999999996
			 */
			double to_double() const 
			{
				std::stringstream ss;
				ss
					<< major()
					<< '.'
					<< minor()
					;

				double r{ 0.0 };
				ss >> r;	//	read double from stream

				return r;
			}
						
		};
	}

	class version : public details::ver<std::uint16_t, std::uint32_t>
	{
	public:
		constexpr version(std::uint16_t maj, std::uint16_t min)
			: this_type(maj, min)
		{}

		constexpr version(std::uint32_t v)
			: this_type(v)
		{}

		version(double d);

		constexpr version(version const& v)
			: this_type(v.first, v.second)
		{}

		constexpr version()
			: this_type(0, 0)
		{}
	};
	
	class revision : public details::ver<std::uint32_t, std::uint64_t>
	{
	public:
		constexpr revision(std::uint16_t a, std::uint16_t b, std::uint16_t c, std::uint16_t d)
			: this_type(version(a, b).full(), version(c, d).full())
		{}

		constexpr revision(std::uint32_t maj, std::uint32_t min)
			: this_type(maj, min)
		{}

		constexpr revision(version maj, version min)
			: this_type(maj.full(), min.full())
		{}

		constexpr revision(std::uint64_t v)
			: this_type(v)
		{}

		constexpr revision(revision const& v)
			: this_type(v.first, v.second)
		{}

		constexpr revision()
			: this_type(0, 0)
		{}

	};
		
	//
	//	comparison version
	//
	constexpr bool operator==(version const& lhs, version const& rhs)
	{
		return lhs.full() == rhs.full();
	}
	constexpr bool operator<(version const& lhs, version const& rhs)
	{
		return lhs.is_less(rhs);
	}
	constexpr bool operator!=(version const& lhs, version const& rhs)
	{
		return !(lhs == rhs);
	}
	constexpr bool operator>(version const& lhs, version const& rhs)
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	constexpr bool operator<=(version const& lhs, version const& rhs)
	{
		return !(lhs > rhs);
	}
	constexpr bool operator>=(version const& lhs, version const& rhs)
	{
		return !(lhs < rhs);
	}


	//
	//	comparison revision
	//
	constexpr bool operator==(revision const& lhs, revision const& rhs)
	{
		return lhs.full() == rhs.full();
	}
	constexpr bool operator<(revision const& lhs, revision const& rhs)
	{
		return lhs.is_less(rhs);
	}
	constexpr bool operator!=(revision const& lhs, revision const& rhs)
	{
		return !(lhs == rhs);
	}
	constexpr bool operator>(revision const& lhs, revision const& rhs)
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	constexpr bool operator<=(revision const& lhs, revision const& rhs)
	{
		return !(lhs > rhs);
	}
	constexpr bool operator>=(revision const& lhs, revision const& rhs)
	{
		return !(lhs < rhs);
	}
}

#endif	//	CYNG_INTRINSICS_VERSION_H

