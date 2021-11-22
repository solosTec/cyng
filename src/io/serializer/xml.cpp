/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/io/serializer/xml.hpp>
#include <cyng/io/ostream.h>
#include <cyng/io/io_buffer.h>
#include <cyng/io/serialize.hpp>

#include <iomanip>
#include <algorithm>

#include <boost/uuid/uuid_io.hpp>

namespace cyng {
	namespace io {

		bool serializer <null, XML>::write(pugi::xml_node node, null) {
			node.append_attribute("xsi:nil");
			node.set_value("true");
			return true;
		}

		bool serializer <eod, XML>::write(pugi::xml_node node, eod) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<eod>());
			return node.set_value("EOD");
		}

		bool serializer <bool, XML>::write(pugi::xml_node node, bool v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<bool>());
			return (v)
				? node.append_child(pugi::node_pcdata).set_value("true")
				: node.append_child(pugi::node_pcdata).set_value("false")
				;
		}

		bool serializer <buffer_t, XML>::write(pugi::xml_node node, buffer_t const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<buffer_t>());
			const std::string str = cyng::io::to_hex(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::string, XML>::write(pugi::xml_node node, std::string const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::string>());
			return node.append_child(pugi::node_pcdata).set_value(v.c_str());
		}

		bool serializer <boost::uuids::uuid, XML>::write(pugi::xml_node node, boost::uuids::uuid const& v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<boost::uuids::uuid>());
			const std::string str = boost::uuids::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <char, XML>::write(pugi::xml_node node, char v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::int8_t>());
			const std::string str = std::to_string(+v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::int8_t, XML>::write(pugi::xml_node node, std::int8_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::int8_t>());
			const std::string str = std::to_string(+v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::int16_t, XML>::write(pugi::xml_node node, std::int16_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::int16_t>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::int32_t, XML>::write(pugi::xml_node node, std::int32_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::int32_t>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::int64_t, XML>::write(pugi::xml_node node, std::int64_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::int64_t>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::uint8_t, XML>::write(pugi::xml_node node, std::uint8_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::uint8_t>());
			const std::string str = std::to_string(+v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::uint16_t, XML>::write(pugi::xml_node node, std::uint16_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::uint16_t>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::uint32_t, XML>::write(pugi::xml_node node, std::uint32_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::uint32_t>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <std::uint64_t, XML>::write(pugi::xml_node node, std::uint64_t v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::uint64_t>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <float, XML>::write(pugi::xml_node node, float v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<float>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <double, XML>::write(pugi::xml_node node, double v)
		{
			node.append_attribute("type").set_value(cyng::intrinsic_name<double>());
			const std::string str = std::to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}
		
		bool serializer <std::chrono::system_clock::time_point, XML>::write(pugi::xml_node node, std::chrono::system_clock::time_point const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::chrono::system_clock::time_point>());
			const std::string str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		
		bool serializer <std::filesystem::path, XML>::write(pugi::xml_node node, std::filesystem::path const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<std::filesystem::path>());
			const std::string str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <boost::asio::ip::address, XML>::write(pugi::xml_node node, boost::asio::ip::address const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<boost::asio::ip::address>());
			std::stringstream ss;
			ss << v;
			auto const str = ss.str();
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <mac48, XML>::write(pugi::xml_node node, mac48 const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<mac48>());
			std::stringstream ss;
			ss << v;
			auto const str = ss.str();
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <mac64, XML>::write(pugi::xml_node node, mac64 const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<mac64>());
			std::stringstream ss;
			auto const str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <pid, XML>::write(pugi::xml_node node, pid const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<pid>());
			auto const str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <obis, XML>::write(pugi::xml_node node, obis const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<obis>());
			auto const str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <obis_path_t, XML>::write(pugi::xml_node node, obis_path_t const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<obis_path_t>());
			auto const str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <edis, XML>::write(pugi::xml_node node, edis const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<edis>());
			auto const str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <version, XML>::write(pugi::xml_node node, version const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<version>());
			auto const str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <revision, XML>::write(pugi::xml_node node, revision const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<revision>());
			auto const str = to_string(v);
			return node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		bool serializer <vector_t, XML>::write(pugi::xml_node node, vector_t const& v) {
			node.append_attribute("type").set_value(cyng::intrinsic_name<vector_t>());
			node.append_attribute("size").set_value(std::to_string(v.size()).c_str());

			std::size_t index{ 0 };
			for (auto const& obj : v)
			{
				switch (obj.rtti().tag())
				{
				case TC_TUPLE:
				case TC_VECTOR:
				case TC_DEQUE:
				case TC_ATTR_MAP:
				case TC_PARAM_MAP:
				{
					auto sub = node.append_child("element");
					sub.append_attribute("index").set_value(index);
					serialize<XML>::write(sub, obj);
				}
				break;
				case type_code::TC_PARAM:
					serialize<XML>::write(node, obj);
					break;
				default:
					serialize<XML>::write(node.append_child("value"), obj);
					break;
				}
				++index;
			}
			return true;
		}

		bool serializer <tuple_t, XML>::write(pugi::xml_node node, tuple_t const& v) {
			//
			//	construct a unique node name
			//
			std::stringstream ss;
			ss
				<< cyng::intrinsic_name<tuple_t>()
				<< '_'
				<< std::hex
				<< std::hash<uintptr_t>()(reinterpret_cast<uintptr_t>(&v))
				;

			auto const type_name = ss.str();
			node.append_attribute("type").set_value(cyng::intrinsic_name<tuple_t>());
			node.append_attribute("size").set_value(std::to_string(v.size()).c_str());

			if (v.size() > 1)
			{
				for (auto const& obj : v)
				{
					switch (obj.rtti().tag())
					{
					case TC_TUPLE:
					case TC_VECTOR:
					case TC_DEQUE:
					case TC_ATTR_MAP:
					case TC_PARAM_MAP:
						serialize<XML>::write(node.append_child(type_name.c_str()), obj);
						break;
					case type_code::TC_PARAM:
						serialize<XML>::write(node, obj);
						break;
					default:
						serialize<XML>::write(node.append_child("value"), obj);
						break;
					}
				}
			}
			else if (v.size() == 1)
			{
				switch (v.front().rtti().tag())
				{
				case TC_TUPLE:
				case TC_VECTOR:
				case TC_DEQUE:
				case TC_ATTR_MAP:
				case TC_PARAM_MAP:
					serialize<XML>::write(node.append_child(type_name.c_str()), v.front());
					break;
				case type_code::TC_PARAM:
					serialize<XML>::write(node, v.front());
					break;
				default:
					serialize<XML>::write(node.append_child("value"), v.front());
					break;
				}
			}
			return true;
		}

		bool serializer <deque_t, XML>::write(pugi::xml_node, deque_t const& v) {
			return false;
		}

		bool serializer <param_t, XML>::write(pugi::xml_node node, param_t const& v) {
			auto child = node.append_child(cyng::intrinsic_name<param_t>());
			child.append_attribute("name").set_value(v.first.c_str());
			return serialize<XML>::write(child, v.second);
		}

		bool serializer <attr_t, XML>::write(pugi::xml_node node, attr_t const& v) {
			pugi::xml_node child = node.append_child(std::to_string(v.first).c_str());
			child.append_attribute("type").set_value(cyng::intrinsic_name<attr_t>());
			return serialize<XML>::write(child, v.second);
		}

		bool serializer <param_map_t, XML>::write(pugi::xml_node node, param_map_t const& v) {
			for (auto const& p : v)	{
				serializer<param_t, XML>::write(node, p);
			}
			return true;
		}

		bool serializer <attr_map_t, XML>::write(pugi::xml_node node, attr_map_t const& v) {
			for (auto const& p : v) {
				serializer<attr_t, XML>::write(node, p);
			}
			return true;
		}

	}
}
