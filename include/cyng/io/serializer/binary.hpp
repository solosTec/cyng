/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_BINARY_HPP
#define CYNG_IO_BINARY_HPP

#include <cyng/io/io.h>
#include <cyng/obj/core/type.hpp>
#include <cyng/obj/buffer_cast.hpp>

#include <type_traits>
#include <iostream>
#ifdef _DEBUG
#include <cyng/io/ostream.h>
#endif

#include <boost/numeric/conversion/cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/icmp.hpp>
#include <boost/system/error_code.hpp>

namespace cyng {
	namespace io {

		/**
		 * @brief Write the length of the value.
		 *
		 * The same encoding as for websockets is used.
		 *
		 * @return the effective size of the length field (1, 3 or 9)
		 */
		std::size_t serialize_length(std::ostream& os, std::size_t size);

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
		std::size_t write_binary(std::ostream& os, T const& v)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");

			auto const buffer = to_buffer<T, N, OFFSET>(v);

			//
			//	check network byte ordering
			//
			os.write(buffer.data(), buffer.size());
			return buffer.size();
		}

		template < typename T, std::size_t N, std::size_t OFFSET = 0 >
		std::size_t write_binary(std::ostream& os, std::array< T, N > const& a)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");

			using array_type = std::array< T, N >;
			static_assert(N + OFFSET == std::tuple_size<array_type>::value, "invalid template parameters");
			static_assert(OFFSET <= N, "index out if range");

			using offset_t = std::integral_constant<std::size_t, OFFSET * sizeof(T)>;
			using length_t = std::integral_constant<std::size_t, (N - OFFSET) * sizeof(T) >;

			os.write(reinterpret_cast<const std::ostream::char_type*>(a.data()) + offset_t::value, length_t::value);
			return length_t::value;
		}

		template < typename T, std::size_t N, std::size_t OFFSET = 0 >
		std::size_t  write_binary(std::ostream& os, T const(&p)[N])
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");

			using offset_t = std::integral_constant<std::size_t, OFFSET * sizeof(T)>;
			using length_t = std::integral_constant<std::size_t, (N - OFFSET) * sizeof(T) >;
			static_assert(OFFSET <= N, "index out if range");

			os.write(reinterpret_cast<const std::ostream::char_type*>(p) + offset_t::value, length_t::value);
			return length_t::value;
		}

		namespace {

			/**
			 * calculate type tag and serialize it as uint32_t data type.
			 */
			template < typename T >
			void serialize_type_tag(std::ostream& os)
			{
				//	could throw
				try {
					std::uint16_t const tag = boost::numeric_cast<std::uint16_t>(type_tag_traits<T>());
					write_binary(os, tag);
				}
				catch (boost::numeric::bad_numeric_cast const& ex) {

#ifdef _DEBUG
					std::cerr
						<< std::endl
						<< "*** error serialize_type_tag: "
						<< ex.what()
						<< std::endl
						;
#endif

					std::uint16_t const tag = 0;
					write_binary(os, tag);
				}
			}
		}

		/**
		 * Generic binary serializer
		 */
		template <typename T>
		struct serializer<T, BINARY>
		{
			static std::size_t write(std::ostream& os, T const& v)
			{
				//
				//	type as std::uint32_t
				//
				serialize_type_tag<T>(os);

				//
				//	length
				//
				auto const ll = serialize_length(os, sizeof(T));

				//
				//	value
				//
				return write_binary(os, v) + sizeof(std::uint16_t) + ll;
			}
		};

		template <>
		struct serializer <null, BINARY>
		{
			static std::size_t write(std::ostream& os, null);
		};
		template <>
		struct serializer <eod, BINARY>
		{
			static std::size_t write(std::ostream& os, eod);
		};
		template <>
		struct serializer <bool, BINARY>
		{
			static std::size_t write(std::ostream& os, bool v);
		};
		template <>
		struct serializer <std::string, BINARY>
		{
			static std::size_t write(std::ostream& os, std::string const&);
		};
		template <>
		struct serializer <std::filesystem::path, BINARY>
		{
			static std::size_t write(std::ostream& os, std::filesystem::path const&);
		};
		template <>
		struct serializer <boost::asio::ip::address, BINARY>
		{
			static std::size_t write(std::ostream& os, boost::asio::ip::address const&);
		};
		template<typename T>
		struct serializer <boost::asio::ip::basic_endpoint<T>, BINARY>
		{
			static std::size_t write(std::ostream& os, boost::asio::ip::basic_endpoint<T> const& ep) {

				auto const address = ep.address().to_string();
				auto const port = ep.port();

				//
				//	type - length - data
				//
				serialize_type_tag<boost::asio::ip::basic_endpoint<T>>(os);
				auto const ll = serialize_length(os, address.size() + sizeof(port));
				write_binary(os, ep.port());
				os << address;
				return address.size() + sizeof(port) + sizeof(std::uint16_t) + ll;
			}
		};
		template <>
		struct serializer <std::chrono::system_clock::time_point, BINARY>
		{
			static std::size_t write(std::ostream& os, std::chrono::system_clock::time_point const&);
		};
		/**
		 * Write a timespan in hh::mm::ss.ffff format.
		 *
		 * @tparam R an arithmetic type representing the number of ticks
		 * @tparam P a std::ratio representing the tick period (i.e. the number of seconds per tick)
		 */
		template <typename R, typename P>
		struct serializer <std::chrono::duration<R, P>, BINARY>
		{
			using type = typename std::chrono::duration<R, P>;
			static std::size_t write(std::ostream& os, std::chrono::duration<R, P> const& v) {

				serialize_type_tag<type>(os);
				auto const ll = serialize_length(os, sizeof(R));	
				try
				{
					return write_binary(os, v.count()) + sizeof(std::uint16_t) + ll;
					//return write_binary(os, boost::numeric_cast<std::int64_t>(v.count())) + sizeof(std::uint16_t) + ll;
				}
				catch (boost::numeric::negative_overflow const&)
				{
					return write_binary(os, std::numeric_limits<std::int64_t>::lowest()) + sizeof(std::uint16_t) + ll;
				}
				catch (boost::numeric::positive_overflow const&)
				{
					return write_binary(os, std::numeric_limits<std::int64_t>::max()) + sizeof(std::uint16_t) + ll;
				}
				catch (...)
				{
				}
				return sizeof(std::uint16_t) + ll;
			}
		};
		template <>
		struct serializer <boost::uuids::uuid, BINARY>
		{
			static std::size_t write(std::ostream& os, boost::uuids::uuid const&);
		};
		template <>
		struct serializer <boost::system::error_code, BINARY>
		{
			static std::size_t write(std::ostream& os, boost::system::error_code const&);
		};

		template <>
		struct serializer <mac48, BINARY>
		{
			static std::size_t write(std::ostream& os, mac48 const&);
		};
		template <>
		struct serializer <mac64, BINARY>
		{
			static std::size_t write(std::ostream& os, mac64 const&);
		};

		template <>
		struct serializer <pid, BINARY>
		{
			static std::size_t write(std::ostream& os, pid const&);
		};

		template <>
		struct serializer <obis, BINARY>
		{
			static std::size_t write(std::ostream& os, obis const&);
		};

		template <>
		struct serializer <edis, BINARY>
		{
			static std::size_t write(std::ostream& os, edis const&);
		};

		template <>
		struct serializer <version, BINARY>
		{
			static std::size_t write(std::ostream& os, version const&);
		};
		template <>
		struct serializer <revision, BINARY>
		{
			static std::size_t write(std::ostream& os, revision const&);
		};

		template <>
		struct serializer <buffer_t, BINARY>
		{
			static std::size_t write(std::ostream& os, buffer_t const&);
		};


		template <>
		struct serializer <attr_t, BINARY>
		{
			static std::size_t write(std::ostream& os, attr_t const&);
		};
		template <>
		struct serializer <param_t, BINARY>
		{
			static std::size_t write(std::ostream& os, param_t const&);
		};
		template <>
		struct serializer <attr_map_t, BINARY>
		{
			static std::size_t write(std::ostream& os, attr_map_t const&);
		};
		template <>
		struct serializer <param_map_t, BINARY>
		{
			static std::size_t write(std::ostream& os, param_map_t const&);
		};
		template <>
		struct serializer <tuple_t, BINARY>
		{
			static std::size_t write(std::ostream& os, tuple_t const&);
		};
		template <>
		struct serializer <vector_t, BINARY>
		{
			static std::size_t write(std::ostream& os, vector_t const&);
		};
		template <>
		struct serializer <deque_t, BINARY>
		{
			static std::size_t write(std::ostream& os, deque_t const&);
		};

		template <std::size_t N>
		struct serializer <digest<N>, BINARY>
		{
			static std::size_t write(std::ostream& os, digest<N> const& d) {

				serialize_type_tag<digest<N>>(os);
				auto const ll = serialize_length(os, digest<N>::size());
				return write_binary(os, d.digest_) + sizeof(std::uint16_t) + ll;

			}
		};

		template <std::size_t N>
		struct serializer <aes_key<N>, BINARY>
		{
			static std::size_t write(std::ostream& os, aes_key<N> const& key) {

				serialize_type_tag<aes_key<N>>(os);
				auto const ll = serialize_length(os, aes_key<N>::bytes());
				return write_binary(os, key.key_) + sizeof(std::uint16_t) + ll;

			}
		};

	}
}
#endif
