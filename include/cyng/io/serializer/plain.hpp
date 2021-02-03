/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_PLAIN_HPP
#define CYNG_IO_PLAIN_HPP

#include <cyng/io/io.h>
#include <boost/uuid/uuid.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/icmp.hpp>

namespace cyng {
	namespace io {

		template <>
		struct serializer <bool, PLAIN>
		{
			static std::size_t write(std::ostream& os, bool v);
		};

		template <>
		struct serializer <boost::uuids::uuid, PLAIN>
		{
			static std::size_t write(std::ostream& os, boost::uuids::uuid const& v);
		};

		template <>
		struct serializer <std::int8_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::int8_t v);
		};

		template <>
		struct serializer <std::int16_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::int16_t v);
		};

		template <>
		struct serializer <std::int32_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::int32_t v);
		};

		template <>
		struct serializer <std::int64_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::int64_t v);
		};

		template <>
		struct serializer <std::uint8_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::uint8_t v);
		};

		template <>
		struct serializer <std::uint16_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::uint16_t v);
		};

		template <>
		struct serializer <std::uint32_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::uint32_t v);
		};

		template <>
		struct serializer <std::uint64_t, PLAIN>
		{
			static std::size_t write(std::ostream& os, std::uint64_t v);
		};

		template <>
		struct serializer <double, PLAIN>
		{
			static std::size_t write(std::ostream& os, double v);
		};

		template<typename T>
		struct serializer <boost::asio::ip::basic_endpoint<T>, PLAIN>
		{
			static std::size_t write(std::ostream& os, boost::asio::ip::basic_endpoint<T> const& ep) {

				auto const address = ep.address().to_string();
				auto const port = ep.port();

				//
				//	address:port
				//
				os
					<< address
					<< ':'
					<< port
					;

				return address.size() + sizeof(port) + 1;
			}
		};

	}
}
#endif
