/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/xml.h>
#include <cyng/object.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/core/class_interface.h>
#include <cyng/xml/serializer.hpp>
#include <cyng/factory.h>
#include <cyng/xml/reader.hpp>

namespace cyng 
{
	namespace xml 
	{
		
		template <typename T>
		void do_write(pugi::xml_node node, object const& obj)
		{
			using serial_t = serializer<T>;
			auto p = object_cast<T>(obj);
			if (p != nullptr)	serial_t::out(node, *p);
		}

		void write(pugi::xml_node node, object const& obj)
		{
			//
			//	no meta data available
			//
			if (!obj)	return;

			//
			//	convert from runtime to compile time
			//
			switch (obj.get_class().tag())
			{
			case type_code::TC_NULL:
				do_write<typename std::tuple_element<type_code::TC_NULL, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_BOOL:
				do_write<typename std::tuple_element<type_code::TC_BOOL, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_CHAR:
				do_write<typename std::tuple_element<type_code::TC_CHAR, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_FLOAT:
				do_write<typename std::tuple_element<type_code::TC_FLOAT, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_DOUBLE:
				do_write<typename std::tuple_element<type_code::TC_DOUBLE, cyng::traits::tag_t>::type>(node, obj);
				break;
				// 			// 		const char*,
			case type_code::TC_UINT8:
				do_write<typename std::tuple_element<type_code::TC_UINT8, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_UINT16:
				do_write<typename std::tuple_element<type_code::TC_UINT16, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_UINT32:
				do_write<typename std::tuple_element<type_code::TC_UINT32, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_UINT64:
				do_write<typename std::tuple_element<type_code::TC_UINT64, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_INT8:
				do_write<typename std::tuple_element<type_code::TC_INT8, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_INT16:
				do_write<typename std::tuple_element<type_code::TC_INT16, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_INT32:
				do_write<typename std::tuple_element<type_code::TC_INT32, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_INT64:
				do_write<typename std::tuple_element<type_code::TC_INT64, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_STRING:
				do_write<typename std::tuple_element<type_code::TC_STRING, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_TIME_POINT:
				do_write<typename std::tuple_element<type_code::TC_TIME_POINT, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_NANO_SECOND:
				do_write<typename std::tuple_element<type_code::TC_NANO_SECOND, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_MICRO_SECOND:
				do_write<typename std::tuple_element<type_code::TC_MICRO_SECOND, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_MILLI_SECOND:
				do_write<typename std::tuple_element<type_code::TC_MILLI_SECOND, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_SECOND:
				do_write<typename std::tuple_element<type_code::TC_SECOND, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_MINUTE:
				do_write<typename std::tuple_element<type_code::TC_MINUTE, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_HOUR:
				do_write<typename std::tuple_element<type_code::TC_HOUR, cyng::traits::tag_t>::type>(node, obj);
				break;
				// 
			case type_code::TC_DBL_TP:
				do_write<typename std::tuple_element<type_code::TC_DBL_TP, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_VERSION:
				do_write<typename std::tuple_element<type_code::TC_VERSION, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_REVISION:
				do_write<typename std::tuple_element<type_code::TC_REVISION, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_CODE:
				do_write<typename std::tuple_element<type_code::TC_CODE, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_BUFFER:
				do_write<typename std::tuple_element<type_code::TC_BUFFER, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_MAC48:
				do_write<typename std::tuple_element<type_code::TC_MAC48, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_MAC64:
				do_write<typename std::tuple_element<type_code::TC_MAC64, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_COLOR_8:
				do_write<typename std::tuple_element<type_code::TC_COLOR_8, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_COLOR_16:
				do_write<typename std::tuple_element<type_code::TC_COLOR_16, cyng::traits::tag_t>::type>(node, obj);
				break;

				// 			
			case type_code::TC_TUPLE:
				do_write<typename std::tuple_element<type_code::TC_TUPLE, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_VECTOR:
				do_write<typename std::tuple_element<type_code::TC_VECTOR, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_SET:
				do_write<typename std::tuple_element<type_code::TC_SET, cyng::traits::tag_t>::type>(node, obj);
				break;
				// 			// 
			case type_code::TC_ATTR_MAP:
				do_write<typename std::tuple_element<type_code::TC_ATTR_MAP, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_ATTR:
				do_write<typename std::tuple_element<type_code::TC_ATTR, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_PARAM_MAP:
				do_write<typename std::tuple_element<type_code::TC_PARAM_MAP, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_PARAM:
				do_write<typename std::tuple_element<type_code::TC_PARAM, cyng::traits::tag_t>::type>(node, obj);
				break;

			case type_code::TC_EC:
				do_write<typename std::tuple_element<type_code::TC_EC, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_UUID:
				do_write<typename std::tuple_element<type_code::TC_UUID, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_FS_PATH:
				do_write<typename std::tuple_element<type_code::TC_FS_PATH, cyng::traits::tag_t>::type>(node, obj);
				break;

			case type_code::TC_IP_TCP_ENDPOINT:
				do_write<typename std::tuple_element<type_code::TC_IP_TCP_ENDPOINT, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_IP_UDP_ENDPOINT:
				do_write<typename std::tuple_element<type_code::TC_IP_UDP_ENDPOINT, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_IP_ICMP_ENDPOINT:
				do_write<typename std::tuple_element<type_code::TC_IP_ICMP_ENDPOINT, cyng::traits::tag_t>::type>(node, obj);
				break;
			case type_code::TC_IP_ADDRESS:
				do_write<typename std::tuple_element<type_code::TC_IP_ADDRESS, cyng::traits::tag_t>::type>(node, obj);
				break;

			case type_code::TC_EOD:
				do_write<typename std::tuple_element<type_code::TC_EOD, cyng::traits::tag_t>::type>(node, obj);
				break;
			default:
				//std::cerr << "unknown type code: " << obj.get_class().tag() << ", " << obj.get_class().type_name() << std::endl;
				//do_write_custom<S>(os, obj);
				break;
			}
			return;
		}

		void write(pugi::xml_node node, tuple_t const& tpl) {
			using serial_t = serializer<tuple_t>;
			serial_t::out(node, tpl);
		}

		//object read(std::string const& inp)
		//{
		//	std::pair<object, bool> r = parse_json(inp);
		//	return (r.second)
		//	? r.first
		//	: make_object()
		//	;
		//}
		//
		//object read(buffer_t const& inp)
		//{
		//	std::pair<object, bool> r = parse_json(inp);
		//	return (r.second)
		//	? r.first
		//	: make_object()
		//	;
		//}
		//
		object read_file(std::string const& p)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(p.c_str());
			if (result) {
	
				auto root = doc.first_child();
				auto type = root.attribute("type").as_string();

				return read_root(root);
				//if (boost::algorithm::equals(type, "vec")) {
				//	std::cout
				//		<< root.name()
				//		<< " type: "
				//		<< type
				//		<< " size: "
				//		<< root.attribute("size").as_int()
				//		<< std::endl;

				//	vector_t vec;
				//	vec.reserve(root.attribute("size").as_int());
				//	for (pugi::xml_node elem : root) {
				//		std::cout << elem.name() << std::endl;
				//		vec.push_back(read(elem));
				//	}

				//	return make_object(vec);

				//}
				//else {
				//	std::cout
				//		<< root.name()
				//		<< " type: "
				//		<< type
				//		<< std::endl;

				//}
				//return read(root.begin(), root.end());

			}

			std::stringstream ss;
			ss
				<< "XML [" 
				<< p 
				<< "] parsed with errors: [" 
				<< result.description()
				<< "]\n"
				<< "Error offset: " 
				<< result.offset
				;
			return make_object(ss.str());
		}

	}
}

