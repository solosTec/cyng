/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_OBJECT_READER_HPP
#define CYNG_IO_OBJECT_READER_HPP

#include <cyng/intrinsics.h>
#include <istream>
#include <type_traits>
#include <boost/numeric/conversion/cast.hpp>

namespace cyng 
{
	namespace io 
	{
		/**
		 * Reads the value binary from a stream into
		 * a temporary variable of type T. Only allowed for 
		 * arithemtic types and enums.
		 * 
		 * @tparam T object to read
		 * @tparam N bytes to read
		 * @see write_binary()
		 */
		template < typename T, std::size_t N = sizeof(T) >
		T read_binary(std::istream& is)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");
			static_assert(sizeof(T) == N, "type size exceeded");
			T v = T();
			is.read(reinterpret_cast<std::istream::char_type*>(&v), N);
			return v;
		}
		
		/**
		 * read an array of elements
		 */
		template < typename T, std::size_t N >
		void read_binary(std::istream& is, T (&p)[N])
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");
			is.read(reinterpret_cast<std::istream::char_type*>(p), sizeof(T) * N);
			BOOST_ASSERT_MSG(is.gcount() == sizeof(T) * N, "read failed");
		}
		
		/**
		 * read an array of elements
		 */
		template < typename T, std::size_t N >
		void read_binary(std::istream& is, std::array< T, N >& a)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");
			is.read(reinterpret_cast<std::istream::char_type*>(a.data()), sizeof(T) * a.size());
			BOOST_ASSERT_MSG(is.gcount() == sizeof(T) * a.size(), "read failed");
		}

		/**
		 * Specialize this policy for your custom data types 
		 */
		template <typename T>
		struct reader_policy
		{
			using type = typename std::decay<T>::type;
			
			static T extract(std::istream& is)
			{
				return read_binary<T>(is);
// 				T v;
// 				is >> v;
// 				return v;
			}
		};
	
		template <>
		struct reader_policy<std::chrono::system_clock::time_point>
		{
			static std::chrono::system_clock::time_point extract(std::istream& is);
		};
		
		/**
		 * Read a duration using the binary protocol. All ticks saved as signed 64 bit 
		 * integer to be platform neutral. (Microsoft Visual C++ version 14.0 uses 4 bytes, gcc++ 6 uses
		 * 8 bytes).
		 * 
		 * @tparam R an arithmetic type representing the number of ticks
		 * @tparam P a std::ratio representing the tick period (i.e. the number of seconds per tick)
		 */
		template <typename R, typename P>
		struct reader_policy < std::chrono::duration<R, P> >
		{
			using value_type = std::chrono::duration<R, P>;
			
			static value_type extract(std::istream& is)
			{
				const std::int64_t ticks = read_binary<std::int64_t>(is);
				try
				{
					return value_type(boost::numeric_cast<R>(ticks));
				}
				catch (boost::numeric::negative_overflow const&)
				{
					return value_type(std::numeric_limits<R>::lowest());
				}
				catch (boost::numeric::positive_overflow const&)
				{
					return value_type(std::numeric_limits<R>::max());
				}
				catch (...)
				{
				}

				return value_type(0);
			}
		};
		
		template <>
		struct reader_policy<std::string>
		{
			static std::string extract(std::istream& is);
		};
		
		template <>
		struct reader_policy<boost::uuids::uuid>
		{
			static boost::uuids::uuid extract(std::istream& is);
		};
		
		template <>
		struct reader_policy<boost::filesystem::path>
		{
			static boost::filesystem::path extract(std::istream& is);
		};

		template <>
		struct reader_policy<chrono::dbl_time_point>
		{
			static chrono::dbl_time_point extract(std::istream& is);
		};
		
		template <>
		struct reader_policy<version>
		{
			static version extract(std::istream& is);
		};

		template <>
		struct reader_policy<revision>
		{
			static revision extract(std::istream& is);
		};

		template <>
		struct reader_policy<code>
		{
			static code extract(std::istream& is);
		};

		template <>
		struct reader_policy<buffer_t>
		{
			static buffer_t extract(std::istream& is);
		};

		template <>
		struct reader_policy<mac48>
		{
			static mac48 extract(std::istream& is);
		};

		template <>
		struct reader_policy<mac64>
		{
			static mac64 extract(std::istream& is);
		};

		template <>
		struct reader_policy<crypto::digest_md5>
		{
			static crypto::digest_md5 extract(std::istream& is);
		};

		template <>
		struct reader_policy<crypto::digest_sha1>
		{
			static crypto::digest_sha1 extract(std::istream& is);
		};

		template <>
		struct reader_policy<crypto::digest_sha256>
		{
			static crypto::digest_sha256 extract(std::istream& is);
		};

		template <>
		struct reader_policy<crypto::digest_sha512>
		{
			static crypto::digest_sha512 extract(std::istream& is);
		};


	}
	
	/**
	 * Create an object instance from an input stream 
	 * 
	 * @tparam T data type to create - default constructor required
	 */
	template <typename T>
	T read_object(std::istream& is)
	{
		using policy = typename io::reader_policy<T>;
		return policy::extract(is);
	}
}

#endif 	//	CYNG_IO_OBJECT_READER_HPP

