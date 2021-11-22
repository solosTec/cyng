/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_XML_HPP
#define CYNG_IO_XML_HPP

#include <cyng/io/io.h>
#include <cyng/obj/intrinsics/color.hpp>
#include <cyng/obj/intrinsics/null.h>
#include <cyng/obj/intrinsics/eod.h>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/intrinsics/mac.h>
#include <cyng/obj/intrinsics/pid.h>
#include <cyng/obj/intrinsics/digest.hpp>
#include <cyng/obj/intrinsics/aes_key.hpp>
#include <cyng/obj/tag.hpp>

#include <cyng/io/serialize.hpp>
#include <pugixml.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/ip/icmp.hpp>

namespace cyng {
	namespace io {

		template <>
		struct serialize<XML>
		{
			template <typename T>
			static bool write_impl(pugi::xml_node node, object const& obj)
			{
				//
				//	select a serializer
				//
				using serial_t = serializer <T, XML>;
				auto ptr = object_cast<T>(obj);
				return (ptr != nullptr)
					? serial_t::write(node, *ptr)
					: 0
					;

			}

			static bool write(pugi::xml_node node, object const& obj);
		};

		template <>
		struct serializer <null, XML>
		{
			static bool write(pugi::xml_node node, null);
		};

		template <>
		struct serializer <eod, XML>
		{
			static bool write(pugi::xml_node node, eod);
		};

		template <>
		struct serializer <bool, XML>
		{
			static bool write(pugi::xml_node node, bool v);
		};

		template <>
		struct serializer <buffer_t, XML>
		{
			static bool write(pugi::xml_node node, buffer_t const& v);
		};

		template <>
		struct serializer <std::string, XML>
		{
			static bool write(pugi::xml_node, std::string const& v);
		};

		template <>
		struct serializer <boost::uuids::uuid, XML>
		{
			static bool write(pugi::xml_node node, boost::uuids::uuid const& v);
		};

		template <>
		struct serializer <char, XML>
		{
			static bool write(pugi::xml_node node, char v);
		};

		template <>
		struct serializer <std::int8_t, XML>
		{
			static bool write(pugi::xml_node node, std::int8_t v);
		};

		template <>
		struct serializer <std::int16_t, XML>
		{
			static bool write(pugi::xml_node node, std::int16_t v);
		};

		template <>
		struct serializer <std::int32_t, XML>
		{
			static bool write(pugi::xml_node node, std::int32_t v);
		};

		template <>
		struct serializer <std::int64_t, XML>
		{
			static bool write(pugi::xml_node node, std::int64_t v);
		};

		template <>
		struct serializer <std::uint8_t, XML>
		{
			static bool write(pugi::xml_node node, std::uint8_t v);
		};

		template <>
		struct serializer <std::uint16_t, XML>
		{
			static bool write(pugi::xml_node node, std::uint16_t v);
		};

		template <>
		struct serializer <std::uint32_t, XML>
		{
			static bool write(pugi::xml_node node, std::uint32_t v);
		};

		template <>
		struct serializer <std::uint64_t, XML>
		{
			static bool write(pugi::xml_node node, std::uint64_t v);
		};

		template <>
		struct serializer <float, XML>
		{
			static bool write(pugi::xml_node node, float v);
		};

		template <>
		struct serializer <double, XML>
		{
			static bool write(pugi::xml_node node, double v);
		};

		template<typename T>
		struct serializer <boost::asio::ip::basic_endpoint<T>, XML>
		{
			using type = boost::asio::ip::basic_endpoint<T>;

			static bool write(pugi::xml_node node, type const& ep) {

				node.append_attribute("type").set_value(cyng::intrinsic_name<type>());

				auto const address = ep.address().to_string();
				auto const port = std::to_string(ep.port());

				auto address_node = node.append_child("address");
				address_node.append_child(pugi::node_pcdata).set_value(address.c_str());
				auto port_node = node.append_child("port");
				port_node.append_child(pugi::node_pcdata).set_value(port.c_str());
				return true;
			}
		};

		template <>
		struct serializer <std::chrono::system_clock::time_point, XML>
		{
			static bool write(pugi::xml_node node, std::chrono::system_clock::time_point const&);
		};

		template <>
		struct serializer <std::filesystem::path, XML>
		{
			static bool write(pugi::xml_node node, std::filesystem::path const&);
		};

		template <>
		struct serializer <boost::asio::ip::address, XML>
		{
			static bool write(pugi::xml_node node, boost::asio::ip::address const&);
		};

		/**
		 * Write a timespan in hh::mm::ss.ffff format.
		 *
		 * @tparam R an arithmetic type representing the number of ticks
		 * @tparam P a std::ratio representing the tick period (i.e. the number of seconds per tick)
		 */
		template <typename R, typename P>
		struct serializer <std::chrono::duration<R, P>, XML>
		{
			using type = typename std::chrono::duration<R, P>;
			static std::size_t write(pugi::xml_node node, std::chrono::duration<R, P> const& v) {

				node.append_attribute("type").set_value(cyng::intrinsic_name<type>());
				auto const count = std::to_string(v.count());
				return node.append_child(pugi::node_pcdata).set_value(count.c_str());
			}
		};

		template <>
		struct serializer <vector_t, XML>
		{
			static bool write(pugi::xml_node, vector_t const& v);
		};

		template <>
		struct serializer <tuple_t, XML>
		{
			static bool write(pugi::xml_node, tuple_t const& v);
		};

		template <>
		struct serializer <deque_t, XML>
		{
			static bool write(pugi::xml_node, deque_t const& v);
		};

		template <>
		struct serializer <param_t, XML>
		{
			static bool write(pugi::xml_node, param_t const& v);
		};

		template <>
		struct serializer <attr_t, XML>
		{
			static bool write(pugi::xml_node, attr_t const& v);
		};

		template <>
		struct serializer <param_map_t, XML>
		{
			static bool write(pugi::xml_node, param_map_t const& v);
		};

		template <>
		struct serializer <attr_map_t, XML>
		{
			static bool write(pugi::xml_node, attr_map_t const& v);
		};

		template <>
		struct serializer <mac48, XML>
		{
			static bool write(pugi::xml_node node, mac48 const&);
		};
		template <>
		struct serializer <mac64, XML>
		{
			static bool write(pugi::xml_node node, mac64 const&);
		};

		template <>
		struct serializer <pid, XML>
		{
			static bool write(pugi::xml_node node, pid const&);
		};

		template <>
		struct serializer <obis, XML>
		{
			static bool write(pugi::xml_node node, obis const&);
		};

		template <>
		struct serializer <obis_path_t, XML>
		{
			static bool write(pugi::xml_node node, obis_path_t const&);
		};

		template <>
		struct serializer <edis, XML>
		{
			static bool write(pugi::xml_node node, edis const&);
		};

		template <>
		struct serializer <version, XML>
		{
			static bool write(pugi::xml_node node, version const&);
		};

		template <>
		struct serializer <revision, XML>
		{
			static bool write(pugi::xml_node node, revision const&);
		};

		template <std::size_t N>
		struct serializer <digest<N>, XML>
		{
			using type = digest<N>;
			static bool write(pugi::xml_node node, type const& v) {

				node.append_attribute("type").set_value(cyng::intrinsic_name<type>());
				//const std::string str = to_string(v);
				//return node.append_child(pugi::node_pcdata).set_value(str.c_str());
				return false;
			}
		};

		template <std::size_t N>
		struct serializer <aes_key<N>, XML>
		{
			using type = aes_key<N>;
			static bool write(pugi::xml_node node, aes_key<N> const& key) {

				node.append_attribute("type").set_value(cyng::intrinsic_name<type>());
				//const std::string str = to_string(key);
				//return node.append_child(pugi::node_pcdata).set_value(str.c_str());
				return false;
			}
		};

		template <typename T>
		struct serializer <color<T>, XML>
		{
			using type = color<T>;
			static bool write(pugi::xml_node node, color<T> const& col) {
				node.append_attribute("type").set_value(cyng::intrinsic_name<type>());

				auto red_node = node.append_child("red");
				auto const red_value = std::to_string(col.red());
				red_node.append_child(pugi::node_pcdata).set_value(red_value.c_str());

				auto green_node = node.append_child("green");
				auto const green_value = std::to_string(col.green());
				green_node.append_child(pugi::node_pcdata).set_value(green_value.c_str());

				auto blue_node = node.append_child("blue");
				auto const blue_value = std::to_string(col.blue());
				blue_node.append_child(pugi::node_pcdata).set_value(blue_value.c_str());

				auto opacity_node = node.append_child("opacity");
				auto const opacity_value = std::to_string(col.green());
				opacity_node.append_child(pugi::node_pcdata).set_value(opacity_value.c_str());

				return true;
			}
		};

	}
}
#endif
