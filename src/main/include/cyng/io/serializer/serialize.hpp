/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_SERIALIZE_HPP
#define CYNG_IO_SERIALIZE_HPP

#include <cyng/object.h>
#include <cyng/io/serializer/plain.hpp>
#include <cyng/io/serializer/typed.hpp>
#include <cyng/io/serializer/binary.hpp>
#include <cyng/io/serializer/json.hpp>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/object_cast.hpp>
#include <cyng/meta/meta.hpp>
#include <ostream>

namespace cyng 
{	
	
	namespace io
	{
		template <typename T, typename S>
		void do_write(std::ostream& os, object const& obj)
		{
			using serial_t = serializer <T, S>;
			auto p = object_cast<T>(obj);
			if (p != nullptr)	serial_t::write(os, *p);
		}
		
		template <typename S>
		void do_write_custom(std::ostream& os, std::size_t tag, std::string const& type_name, object const& obj)
		{
			using serial_t = serializer_custom <S>;
			//auto p = object_cast<T>(obj);
			serial_t::write(os, tag, type_name, obj);

			//os
			//	<< "<!"
			//	<< obj.get_class().tag()
			//	<< ':'
			//	<< obj.get_class().type_name()
			//	<< '>'
			//	;
		}

		template <typename S>
		struct serializer_helper
		{
			static void write(std::ostream& os, object const& obj)
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
						do_write<typename std::tuple_element<type_code::TC_NULL, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_BOOL:
						do_write<typename std::tuple_element<type_code::TC_BOOL, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_CHAR:
						do_write<typename std::tuple_element<type_code::TC_CHAR, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_FLOAT:
						do_write<typename std::tuple_element<type_code::TC_FLOAT, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_DOUBLE:
						do_write<typename std::tuple_element<type_code::TC_DOUBLE, cyng::traits::tag_t>::type, S>(os, obj);
						break;
// 			// 		const char*,
					case type_code::TC_UINT8:
						do_write<typename std::tuple_element<type_code::TC_UINT8, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_UINT16:
						do_write<typename std::tuple_element<type_code::TC_UINT16, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_UINT32:
						do_write<typename std::tuple_element<type_code::TC_UINT32, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_UINT64:
						do_write<typename std::tuple_element<type_code::TC_UINT64, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_INT8:
						do_write<typename std::tuple_element<type_code::TC_INT8, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_INT16:
						do_write<typename std::tuple_element<type_code::TC_INT16, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_INT32:
						do_write<typename std::tuple_element<type_code::TC_INT32, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_INT64:
						do_write<typename std::tuple_element<type_code::TC_INT64, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_STRING:
						do_write<typename std::tuple_element<type_code::TC_STRING, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_TIME_POINT:
						do_write<typename std::tuple_element<type_code::TC_TIME_POINT, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_NANO_SECOND:
						do_write<typename std::tuple_element<type_code::TC_NANO_SECOND, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_MICRO_SECOND:
						do_write<typename std::tuple_element<type_code::TC_MICRO_SECOND, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_MILLI_SECOND:
						do_write<typename std::tuple_element<type_code::TC_MILLI_SECOND, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_SECOND:
						do_write<typename std::tuple_element<type_code::TC_SECOND, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_MINUTE:
						do_write<typename std::tuple_element<type_code::TC_MINUTE, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_HOUR:
						do_write<typename std::tuple_element<type_code::TC_HOUR, cyng::traits::tag_t>::type, S>(os, obj);
						break;
// 
					case type_code::TC_DBL_TP:
						do_write<typename std::tuple_element<type_code::TC_DBL_TP, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_VERSION:
						do_write<typename std::tuple_element<type_code::TC_VERSION, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_REVISION:
						do_write<typename std::tuple_element<type_code::TC_REVISION, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_CODE:
						do_write<typename std::tuple_element<type_code::TC_CODE, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_BUFFER:
						do_write<typename std::tuple_element<type_code::TC_BUFFER, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_MAC48:
						do_write<typename std::tuple_element<type_code::TC_MAC48, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_MAC64:
						do_write<typename std::tuple_element<type_code::TC_MAC64, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_COLOR_8:
						do_write<typename std::tuple_element<type_code::TC_COLOR_8, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_COLOR_16:
						do_write<typename std::tuple_element<type_code::TC_COLOR_16, cyng::traits::tag_t>::type, S>(os, obj);
						break;
						
// 			
					case type_code::TC_TUPLE:
						do_write<typename std::tuple_element<type_code::TC_TUPLE, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_VECTOR:
						do_write<typename std::tuple_element<type_code::TC_VECTOR, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_SET:
						do_write<typename std::tuple_element<type_code::TC_SET, cyng::traits::tag_t>::type, S>(os, obj);
						break;
// 			// 
					case type_code::TC_ATTR_MAP:
 						do_write<typename std::tuple_element<type_code::TC_ATTR_MAP, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_ATTR:
						do_write<typename std::tuple_element<type_code::TC_ATTR, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_PARAM_MAP:
 						do_write<typename std::tuple_element<type_code::TC_PARAM_MAP, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_PARAM:
						do_write<typename std::tuple_element<type_code::TC_PARAM, cyng::traits::tag_t>::type, S>(os, obj);
						break;

					case type_code::TC_EC:
						do_write<typename std::tuple_element<type_code::TC_EC, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_UUID:
						do_write<typename std::tuple_element<type_code::TC_UUID, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_FS_PATH:
						do_write<typename std::tuple_element<type_code::TC_FS_PATH, cyng::traits::tag_t>::type, S>(os, obj);
						break;

					case type_code::TC_IP_TCP_ENDPOINT:
						do_write<typename std::tuple_element<type_code::TC_IP_TCP_ENDPOINT, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_IP_UDP_ENDPOINT:
						do_write<typename std::tuple_element<type_code::TC_IP_UDP_ENDPOINT, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_IP_ICMP_ENDPOINT:
						do_write<typename std::tuple_element<type_code::TC_IP_ICMP_ENDPOINT, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					case type_code::TC_IP_ADDRESS:
						do_write<typename std::tuple_element<type_code::TC_IP_ADDRESS, cyng::traits::tag_t>::type, S>(os, obj);
						break;

					case type_code::TC_EOD:
						do_write<typename std::tuple_element<type_code::TC_EOD, cyng::traits::tag_t>::type, S>(os, obj);
						break;
					default:
						//std::cerr << "unknown type code: " << obj.get_class().tag() << ", " << obj.get_class().type_name() << std::endl;
						do_write_custom<S>(os, obj.get_class().tag(), obj.get_class().type_name(), obj);
						break;
				}
			}
		};
	}
}

#endif // CYNG_IO_SERIALIZE_HPP


