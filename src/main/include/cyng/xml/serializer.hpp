/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef NODE_XML_SERIALIZER_HPP
#define NODE_XML_SERIALIZER_HPP


#include <pugixml.hpp>
#include <cyng/intrinsics.h>
#include <cyng/intrinsics/traits/tag_names.hpp>
#include <cyng/io/serializer.h>

namespace cyng
{
	namespace xml
	{
		template <typename T>
		struct serializer
		{
			static void out(pugi::xml_node node, T const& v)
			{
				pugi::xml_node child = node.append_child(pugi::node_pcdata);
				child.append_attribute("type").set_value(cyng::traits::get_tag_name<T>());

				using cyng::io::operator<<;
				std::stringstream ss;
				ss << v;
				child.set_value(ss.str().c_str());
				//child.set_value("ToDo");
			}
		};

		template <>
		struct serializer <null>
		{
			static void out(pugi::xml_node, null v);
		};

		template <>
		struct serializer <eod>
		{
			static void out(pugi::xml_node, eod v);
		};

		template <>
		struct serializer <bool>
		{
			static void out(pugi::xml_node, bool v);
		};

		template <>
		struct serializer <buffer_t>
		{
			static void out(pugi::xml_node, buffer_t const& v);
		};

		template <>
		struct serializer <vector_t>
		{
			static void out(pugi::xml_node, vector_t const& v);
		};

		template <>
		struct serializer <tuple_t>
		{
			static void out(pugi::xml_node, tuple_t const& v);
		};

		template <>
		struct serializer <set_t>
		{
			static void out(pugi::xml_node, set_t const& v);
		};

		template <>
		struct serializer <param_t>
		{
			static void out(pugi::xml_node, param_t const& v);
		};

		template <>
		struct serializer <attr_t>
		{
			static void out(pugi::xml_node, attr_t const& v);
		};

		template <>
		struct serializer <param_map_t>
		{
			static void out(pugi::xml_node, param_map_t const& v);
		};

		template <>
		struct serializer <attr_map_t>
		{
			static void out(pugi::xml_node, attr_map_t const& v);
		};

		template <>
		struct serializer <boost::uuids::uuid>
		{
			static void out(pugi::xml_node, boost::uuids::uuid v);
		};

		template <>
		struct serializer <std::string>
		{
			static void out(pugi::xml_node, std::string const& v);
		};

		template <>
		struct serializer <std::chrono::system_clock::time_point>
		{
			static void out(pugi::xml_node, std::chrono::system_clock::time_point v);
		};

		template <>
		struct serializer <color_8>
		{
			static void out(pugi::xml_node, color_8 v);
		};

		template <>
		struct serializer <color_16>
		{
			static void out(pugi::xml_node, color_16 v);
		};

		template <>
		struct serializer <char>
		{
			static void out(pugi::xml_node, char v);
		};

		template <>
		struct serializer <std::uint8_t>
		{
			static void out(pugi::xml_node, std::uint8_t v);
		};
	}
}
#endif
