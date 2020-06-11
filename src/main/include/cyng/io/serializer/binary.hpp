/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZE_BINARY_HPP
#define CYNG_IO_SERIALIZE_BINARY_HPP


#include <cyng/io.h>
#include <cyng/intrinsics.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/io/io_chrono.hpp>
#include <iostream>
#include <iomanip>
#include <boost/numeric/conversion/cast.hpp>

namespace cyng 
{	
	namespace io
	{
		/**
		 * @brief Write the length of the value.
		 * 
		 * The same encoding as for websockets is used.
		 * 
		 * @return the effective size of the length field (1, 3 or 9)
		 */
		std::size_t serialize_length(std::ostream& os, std::size_t);
		
		/**
		 * Take an argument of type size_t and serialize is as uint64_t object
		 * with complete type and length attributes.
		 * This way we can exchange size values between different architectures
		 * in a safe manner.
		 */
		void write_size_as_object(std::ostream& os, std::size_t);

		/**
		 * Copies N last bytes (LSB) from the arithmetic value into the stream. Default is to write all
		 * available bytes. If N exceeds the size of the integral value
		 * the compiler emits an error message.
		 * 
		 * @tparam T object to write
		 * @tparam N bytes to write
		 * @tparam O Offset
		 */
		template < typename T, std::size_t N = sizeof(T), std::size_t OFFSET = 0 >
		void write_binary(std::ostream& os, T const& v)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");
			static_assert(N + OFFSET <= sizeof(T), "type size exceeded");
			os.write(reinterpret_cast<const std::ostream::char_type*>(&v) + OFFSET, N);
		}
		
		template < typename T, std::size_t N, std::size_t OFFSET = 0 >
		void write_binary(std::ostream& os, std::array< T, N > const& a)
		{
			using array_type = std::array< T, N >;
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");
			static_assert(N + OFFSET == std::tuple_size<array_type>::value, "invalid template parameters");
			os.write(reinterpret_cast<const std::ostream::char_type*>(a.data()) + (OFFSET * sizeof(T)), N * sizeof(T));
		}

		template < typename T, std::size_t N, std::size_t OFFSET = 0 >
		void write_binary(std::ostream& os, T const(&p)[N])
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");
			os.write(reinterpret_cast<const std::ostream::char_type*>(p) + (OFFSET * sizeof(T)), sizeof(T) * N);
		}

		/**
		 * calculate type tag and serialize it as uint32_t data type.
		 */
		template < typename T >
		std::ostream& serialize_type_tag(std::ostream& os)
		{
			//	could throw
			try {
				const std::uint32_t tag = boost::numeric_cast<std::uint32_t>(type_tag_traits<T>());
				write_binary(os, tag);
			}
			catch (boost::numeric::positive_overflow const& ex) { 

				std::cerr 
					<< std::endl 
					<< "*** error serialize_type_tag: " 
					<< ex.what() 
					<< std::endl;

				const std::uint32_t tag = 0;
				write_binary(os, tag);
			}
			return os;
		}

		/**
		 * Generic binary serializer
		 */
		template <typename T>
		struct serializer<T, SERIALIZE_BINARY> 
		{		
			static std::ostream& write(std::ostream& os, T const& v)
			{
				//
				//	type
				//
				serialize_type_tag<T>(os);
				
				//
				//	length
				//
				serialize_length(os, sizeof(T));
				
				//
				//	value
				//
				write_binary(os, v);
				return os;
			}
		};
		
 
		template <>
		struct serializer <tuple_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, tuple_t const& v);
		};

		template <>
		struct serializer <vector_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, vector_t const& v);
		};

		template <>
		struct serializer <set_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, set_t const& v);
		};

		template <>
		struct serializer <bool, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, bool v);
		};
		
		template <>
		struct serializer <null, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, null);
		};

		template <>
		struct serializer <std::string, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, std::string const&);
		};

		template <>
		struct serializer <std::chrono::system_clock::time_point, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, std::chrono::system_clock::time_point const&);
		};

		/**
		 * Write a timespan in hh::mm::ss.ffff format.
		 * 
		 * @tparam R an arithmetic type representing the number of ticks
		 * @tparam P a std::ratio representing the tick period (i.e. the number of seconds per tick)
		 */
		template <typename R, typename P>
		struct serializer <std::chrono::duration<R, P>, SERIALIZE_BINARY>
		{
			using type = typename std::chrono::duration<R, P>;
			static std::ostream& write(std::ostream& os, std::chrono::duration<R, P> const& v)
			{
				serialize_type_tag<type>(os);
				serialize_length(os, sizeof(std::int64_t));	//	could be negative
				try
				{
					write_binary(os, boost::numeric_cast<std::int64_t>(v.count()));
				}
				catch (boost::numeric::negative_overflow const&)
				{
					write_binary(os, std::numeric_limits<std::int64_t>::lowest());
				}
				catch (boost::numeric::positive_overflow const&)
				{
					write_binary(os, std::numeric_limits<std::int64_t>::max());
				}
				catch (...)
				{}
				
				return os;
			}
		};

	
// 		[with T = cyng::version; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <version, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, version const&);
		};
		
// 		[with T = cyng::revision; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <revision, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, revision const&);
		};
		
// 		[with T = std::vector<char>; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <char, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, char const&);
		};
		
// 		[with T = cyng::mac48; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <mac48, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, mac48 const&);
		};
		
// 		[with T = cyng::mac64; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <mac64, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, mac64 const&);
		};
				
		template <>
		struct serializer <boost::system::error_code, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, boost::system::error_code const&);
		};

// 		[with T = boost::uuids::uuid; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <boost::uuids::uuid, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, boost::uuids::uuid const&);
		};
		
// 		[with T = filesystem::path; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <filesystem::path, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, filesystem::path const&);
		};
		
// 		[with T = cyng::eod; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <eod, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, eod);
		};
		
		//	[with T = std::pair<long int, double>; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <cyng::chrono::dbl_time_point, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, cyng::chrono::dbl_time_point const&);
		};

		//	[with T = std::vector<char>; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <buffer_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, buffer_t const&);
		};

		//	[with T = cyng::color<unsigned char>; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <color_8, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, color_8 const&);
		};

		//	[with T = cyng::color<short unsigned int>; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <color_16, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, color_16 const&);
		};

		//	[with T = std::pair<long unsigned int, cyng::object>; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <attr_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, attr_t const&);
		};

		//	[with T = std::pair<std::__cxx11::basic_string<char>, cyng::object>; S = cyng::io::SERIALIZE_BINARY; std::ostream = std::basic_ostream<char>]’
		template <>
		struct serializer <param_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, param_t const&);
		};

		template <>
		struct serializer <attr_map_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, attr_map_t const&);
		};

		template <>
		struct serializer <param_map_t, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, param_map_t const&);
		};

		
		template <>
		struct serializer <boost::asio::ip::tcp::endpoint, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::tcp::endpoint const&);
		};

		template <>
		struct serializer <boost::asio::ip::udp::endpoint, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::udp::endpoint const&);
		};

		template <>
		struct serializer <boost::asio::ip::icmp::endpoint, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::icmp::endpoint const&);
		};

		template <>
		struct serializer <boost::asio::ip::address, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, boost::asio::ip::address const&);
		};

		template <>
		struct serializer <crypto::digest_md5, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, crypto::digest_md5 const&);
		};
		template <>
		struct serializer <crypto::digest_sha1, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, crypto::digest_sha1 const&);
		};
		template <>
		struct serializer <crypto::digest_sha256, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, crypto::digest_sha256 const&);
		};
		template <>
		struct serializer <crypto::digest_sha512, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, crypto::digest_sha512 const&);
		};

		template <>
		struct serializer <crypto::aes_128_key, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, crypto::aes_128_key const&);
		};
		template <>
		struct serializer <crypto::aes_192_key, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, crypto::aes_192_key const&);
		};
		template <>
		struct serializer <crypto::aes_256_key, SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, crypto::aes_256_key const&);
		};

		/**
		 * serialize custom types
		 */
		template <>
		struct serializer_custom<SERIALIZE_BINARY>
		{
			static std::ostream& write(std::ostream& os, std::size_t tag, std::string const& type_name, object const& obj);
		};

	}
}

#endif // CYNG_IO_SERIALIZE_BINARY_HPP


