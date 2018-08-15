/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#include <cyng/xml/serializer.hpp>
#include <cyng/io/io_buffer.h>
#include <cyng/io/io_chrono.hpp>
#include <cyng/xml.h>
#include <cyng/core/class_interface.h>

#include <functional>
#include <boost/uuid/uuid_io.hpp>

namespace cyng
{
	namespace xml
	{
		//
		void write_param(pugi::xml_node node, cyng::object obj)
		{
			auto p = object_cast<param_t>(obj);
			if (p != nullptr) {
				auto child = node.append_child(cyng::traits::get_tag_name<param_t>());
				child.append_attribute("name").set_value(p->first.c_str());
				write(child, p->second);
				//write(node.append_child(p->first.c_str()), p->second);
			}
		}

		void serializer <null>::out(pugi::xml_node node, null)
		{
			node.append_attribute("xsi:nil");
			node.set_value("true");
		}

		void serializer <eod>::out(pugi::xml_node node, eod)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<eod>());
			node.set_value("EOD");
		}

		void serializer <bool>::out(pugi::xml_node node, bool v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<bool>());
			if (v)
			{
				node.append_child(pugi::node_pcdata).set_value("true");
			}
			else
			{
				node.append_child(pugi::node_pcdata).set_value("false");
			}
		}

		void serializer <buffer_t>::out(pugi::xml_node node, buffer_t const& v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<buffer_t>());
			const std::string str = cyng::io::to_hex(v);
			node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		void serializer <vector_t>::out(pugi::xml_node node, vector_t const& v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<vector_t>());
			node.append_attribute("size").set_value(std::to_string(v.size()).c_str());

			std::size_t index{ 0 };
			for (auto const& obj : v)
			{
				switch (obj.get_class().tag())
				{
				case TC_TUPLE:
				case TC_VECTOR:
				case TC_SET:
				case TC_ATTR_MAP:
				case TC_PARAM_MAP:
					//write(node.append_child(node_name.c_str()), obj);
				{
					auto sub = node.append_child("element");
					sub.append_attribute("index").set_value(index);
					write(sub, obj);
				}
					break;
				case type_code::TC_PARAM:
					write_param(node, obj);
					break;
				default:
					write(node.append_child("value"), obj);
					break;
				}
				++index;
			}
		}

		void serializer <tuple_t>::out(pugi::xml_node node, tuple_t const& v)
		{
			//
			//	construct a unique node name
			//
			std::stringstream ss;
			ss
				<< cyng::traits::get_tag_name<tuple_t>()
				<< '_'
				<< std::hex
				<< std::hash<uintptr_t>()(reinterpret_cast<uintptr_t>(&v))
				;

			const std::string type_name = ss.str();
			//pugi::xml_node child = node.append_child(type_name.c_str());
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<tuple_t>());
			node.append_attribute("size").set_value(std::to_string(v.size()).c_str());

			if (v.size() > 1)
			{
				for (auto const& obj : v)
				{
					switch (obj.get_class().tag())
					{
					case TC_TUPLE:
					case TC_VECTOR:
					case TC_SET:
					case TC_ATTR_MAP:
					case TC_PARAM_MAP:
						write(node.append_child(type_name.c_str()), obj);
						break;
					case type_code::TC_PARAM:
						write_param(node, obj);
						break;
					default:
						write(node.append_child("value"), obj);
						break;
					}
				}
			}
			else if (v.size() == 1)
			{
				switch (v.front().get_class().tag())
				{
				case TC_TUPLE:
				case TC_VECTOR:
				case TC_SET:
				case TC_ATTR_MAP:
				case TC_PARAM_MAP:
					write(node.append_child(type_name.c_str()), v.front());
					break;
				case type_code::TC_PARAM:
					write_param(node, v.front());
					break;
				default:
					write(node.append_child("value"), v.front());
					break;
				}
			}
		}

		void serializer <set_t>::out(pugi::xml_node node, set_t const& v)
		{
			//
			//	construct a unique node name
			//
			std::stringstream ss;
			ss
				<< cyng::traits::get_tag_name<set_t>()
				<< '_'
				<< std::hex
				<< std::hash<uintptr_t>()(reinterpret_cast<uintptr_t>(&v))
				;

			const std::string type_name = ss.str();
			pugi::xml_node child = node.append_child(type_name.c_str());
			child.append_attribute("type").set_value(cyng::traits::get_tag_name<set_t>());
			child.append_attribute("size").set_value(std::to_string(v.size()).c_str());

			for (auto const& obj : v)
			{
				switch (obj.get_class().tag())
				{
				case TC_TUPLE:
				case TC_VECTOR:
				case TC_SET:
				case TC_ATTR_MAP:
				case TC_PARAM_MAP:
					write(child, obj);
					break;
				case type_code::TC_PARAM:
					write_param(node, obj);
					break;
				default:
					write(child.append_child("value"), obj);
					break;
				}
			}
		}

		void serializer <param_t>::out(pugi::xml_node node, param_t const& v)
		{
			pugi::xml_node child = node.append_child(v.first.c_str());
			write(child, v.second);
		}

		void serializer <attr_t>::out(pugi::xml_node node, attr_t const& v)
		{
			pugi::xml_node child = node.append_child(std::to_string(v.first).c_str());
			child.append_attribute("type").set_value(cyng::traits::get_tag_name<attr_t>());
			write(child, v.second);
		}

		void serializer <param_map_t>::out(pugi::xml_node node, param_map_t const& v)
		{
			for (auto const& p : v)
			{
				serializer< param_t >::out(node, p);
			}
		}

		void serializer <attr_map_t>::out(pugi::xml_node node, attr_map_t const& v)
		{
			for (auto const& p : v)
			{
				serializer< attr_t >::out(node, p);
			}
		}

		void serializer <boost::uuids::uuid>::out(pugi::xml_node node, boost::uuids::uuid v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<boost::uuids::uuid>());
			const std::string str = boost::uuids::to_string(v);
			node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		void serializer <std::string>::out(pugi::xml_node node, std::string const& v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<std::string>());
			node.append_child(pugi::node_pcdata).set_value(v.c_str());
		}

		void serializer <std::chrono::system_clock::time_point>::out(pugi::xml_node node, std::chrono::system_clock::time_point v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<std::chrono::system_clock::time_point>());
			const std::string str = to_str(v);
			node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		void serializer <color_8>::out(pugi::xml_node node, color_8 v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<color_8>());
			node.append_child(pugi::node_pcdata).set_value("ToDo");
		}
		void serializer <color_16>::out(pugi::xml_node node, color_16 v)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<color_16>());
			node.append_child(pugi::node_pcdata).set_value("ToDo");
		}

		void serializer <char>::out(pugi::xml_node node, char c)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<char>());
			const std::string str = std::to_string(+c);
			node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		void serializer <std::uint8_t>::out(pugi::xml_node node, std::uint8_t c)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<std::uint8_t>());
			const std::string str = std::to_string(+c);
			node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}

		void serializer <std::int8_t>::out(pugi::xml_node node, std::int8_t c)
		{
			node.append_attribute("type").set_value(cyng::traits::get_tag_name<std::int8_t>());
			const std::string str = std::to_string(+c);
			node.append_child(pugi::node_pcdata).set_value(str.c_str());
		}
	}
}
