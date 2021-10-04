/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/io/serializer/binary.hpp>
#include <cyng/io/serialize.h>
#include <cyng/obj/object.h>
#include <cyng/obj/factory.hpp>

#include <boost/io/ios_state.hpp>

namespace cyng {
	namespace io {

		std::size_t serialize_length(std::ostream& os, std::size_t size)
		{
			if (size < 0x7e)
			{
				write_binary(os, static_cast<std::uint8_t>(size & 0xFF));
				return sizeof(std::uint8_t);
			}
			else if (size < 0xFFFF)
			{
				write_binary(os, static_cast<std::uint8_t>(0x7e));
				write_binary(os, static_cast<std::uint16_t>(size & 0xFFFF));
				return sizeof(std::uint8_t) + sizeof(std::uint16_t);
			}

			write_binary(os, static_cast<std::uint8_t>(0x7f));
			write_binary(os, static_cast<std::uint64_t>(size));
			return sizeof(std::uint8_t) + sizeof(std::uint64_t);
		}

		std::size_t serializer<null, BINARY>::write(std::ostream& os, null)
		{
			//
			//	type - length - no data
			//
			serialize_type_tag<null>(os);
			auto const ll = serialize_length(os, 0);
			return sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <eod, BINARY>::write(std::ostream& os, eod)
		{
			//
			//	type - length - no data
			//
			serialize_type_tag<eod>(os);
			auto const ll = serialize_length(os, 0);
			return sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <bool, BINARY>::write(std::ostream& os, bool v)
		{
			//
			//	type
			//
			serialize_type_tag<bool>(os);
			std::uint8_t const b = v ? 1 : 0;

			//
			//	length
			//
			auto const ll = serialize_length(os, sizeof(b));

			//
			//	value
			//
			return write_binary(os, b) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer<std::string, BINARY>::write(std::ostream& os, std::string const& str)
		{
			//
			//	type - length - data
			//
			serialize_type_tag<std::string>(os);
			auto const ll = serialize_length(os, str.size());
			os << str;
			return str.size() + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer<std::filesystem::path, BINARY>::write(std::ostream& os, std::filesystem::path const& p)
		{
			//
			//	type - length - data
			//
			serialize_type_tag<std::filesystem::path>(os);
			auto const s = p.generic_string();
			auto const ll = serialize_length(os, s.size());
			os << s;
			return s.size() + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer<boost::asio::ip::address, BINARY>::write(std::ostream& os, boost::asio::ip::address const& addr)
		{
			auto const str = addr.to_string();

			//
			//	type - length - data
			//
			serialize_type_tag<boost::asio::ip::address>(os);
			auto const ll = serialize_length(os, str.size());
			os << str;
			return str.size() + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <boost::uuids::uuid, BINARY>::write(std::ostream& os, boost::uuids::uuid const& v)
		{
			static_assert(sizeof(v) == 16, "invalid assumption");

			//
			//	type - length - value
			//
			serialize_type_tag<boost::uuids::uuid>(os);
			auto const ll = serialize_length(os, sizeof(v));
			os.write(reinterpret_cast<const std::ostream::char_type*>(v.data), sizeof(v));
			return sizeof(v) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer<boost::system::error_code, BINARY>::write(std::ostream& os, boost::system::error_code const& ec) {

			auto const value = ec.value();
			std::string const category = ec.category().name();

			//
			//	type - length - value
			//
			serialize_type_tag<boost::system::error_code>(os);
			auto const ll = serialize_length(os, sizeof(value) + category.size());
			write_binary(os, value);
			os << category;
			return sizeof(value) + category.size() + sizeof(std::uint16_t) + ll;

		}

		std::size_t serializer<std::chrono::system_clock::time_point, BINARY>::write(std::ostream& os, std::chrono::system_clock::time_point const& tp)
		{
			auto const value = tp.time_since_epoch().count();

			//
			//	Write a time point value in binary format as std::time_t.
			//
			serialize_type_tag<std::chrono::system_clock::time_point>(os);
			auto const ll = serialize_length(os, sizeof(value));
			write_binary(os, value);
			return sizeof(value) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <mac48, BINARY>::write(std::ostream& os, mac48 const& v)
		{
			static_assert(sizeof(mac48::address_type) == 6, "invalid assumption");

			//
			//	type - length - value
			//
			serialize_type_tag<mac48>(os);
			auto const ll = serialize_length(os, sizeof(mac48::address_type));
			write_binary(os, v.get_octets());
			return sizeof(mac48::address_type) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <mac64, BINARY>::write(std::ostream& os, mac64 const& v)
		{
			static_assert(sizeof(mac64::address_type) == 8, "invalid assumption");

			//
			//	type - length - value
			//
			serialize_type_tag<mac64>(os);
			auto const ll = serialize_length(os, sizeof(mac64::address_type));
			write_binary(os, v.get_words());
			return sizeof(mac64::address_type) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <pid, BINARY>::write(std::ostream& os, pid const& v)
		{
			using value_type = typename pid::value_type;
			//
			//	type - length - value
			//
			serialize_type_tag<pid>(os);
			auto const ll = serialize_length(os, sizeof(pid::value_type));
			write_binary(os, v.operator value_type());
			return sizeof(value_type) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <obis, BINARY>::write(std::ostream& os, obis const& v)
		{
			static_assert(sizeof(obis::data_type) == obis::size(), "invalid assumption");

			//
			//	type - length - value
			//
			serialize_type_tag<obis>(os);
			auto const ll = serialize_length(os, obis::size());
			write_binary(os, v.data());
			return obis::size() + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <obis_path_t, BINARY>::write(std::ostream& os, obis_path_t const& v)
		{

			//
			//	type - length - value
			//	length is the size of the vector
			//
			serialize_type_tag<obis_path_t>(os);
			auto const ll = serialize_length(os, v.size() * obis::size());
			std::size_t size{ sizeof(std::uint16_t) + ll };
			for (auto const& o : v) {
				size += write_binary(os, o.data());
			}
			return size;
		}
		
		std::size_t serializer <edis, BINARY>::write(std::ostream& os, edis const& v)
		{
			static_assert(sizeof(edis::data_type) == edis::size(), "invalid assumption");

			//
			//	type - length - value
			//
			serialize_type_tag<edis>(os);
			auto const ll = serialize_length(os, edis::size());
			write_binary(os, v.data());
			return edis::size() + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <version, BINARY>::write(std::ostream& os, version const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<version>(os);
			auto const value = v.full();
			auto const ll = serialize_length(os, sizeof(value));
			return write_binary(os, value) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <revision, BINARY>::write(std::ostream& os, revision const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<revision>(os);
			auto const value = v.full();
			auto const ll = serialize_length(os, sizeof(value));
			return write_binary(os, value) + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <buffer_t, BINARY>::write(std::ostream& os, buffer_t const& v)
		{
			//
			//	type - length - value
			//
			serialize_type_tag<buffer_t>(os);
			auto const ll = serialize_length(os, v.size());
			os.write(v.data(), v.size());
			return v.size() + sizeof(std::uint16_t) + ll;
		}

		std::size_t serializer <attr_t, BINARY>::write(std::ostream& os, attr_t const& v)
		{
			//	serialize value
			std::size_t size = serialize_binary(os, v.second);

			//	serialize index as object
			size += serialize_binary(os, make_object(v.first));

			//	serialize instruction
			return size + serialize_binary(os, make_object(op::MAKE_ATTR));
		}

		std::size_t serializer <param_t, BINARY>::write(std::ostream& os, param_t const& v)
		{
			//	serialize value
			std::size_t size = serialize_binary(os, v.second);

			//	serialize name
			size += serialize_binary(os, make_object(v.first));

			//	serialize instruction
			return size + serialize_binary(os, make_object(op::MAKE_PARAM));

		}


		std::size_t serializer <attr_map_t, BINARY>::write(std::ostream& os, attr_map_t const& amap)
		{
			std::size_t size{ 0 };

			//	serialize each element from attribute map
			for (auto const& attr : amap)	{
				size += serializer <attr_t, BINARY>::write(os, attr);
			}

			//	element count as object
			size += serialize_binary(os, make_object<std::size_t>(amap.size()));

			//	serialize instruction
			return size + serialize_binary(os, make_object(op::MAKE_ATTR_MAP));
		}

		std::size_t serializer <param_map_t, BINARY>::write(std::ostream& os, param_map_t const& pmap)
		{
			std::size_t size{ 0 };

			//	serialize each element from parameter map
			for (auto const& param : pmap) {
				size += serializer <param_t, BINARY>::write(os, param);
			}

			//	element count
			size += serialize_binary(os, make_object<std::size_t>(pmap.size()));

			//	serialize instruction
			return size + serialize_binary(os, make_object(op::MAKE_PARAM_MAP));
		}

		std::size_t serializer <tuple_t, BINARY>::write(std::ostream& os, tuple_t const& v)
		{
			std::size_t size{ 0 };

			//	serialize each element from the tuple
			for (auto const& obj : v) {
				size += serialize_binary(os, obj);
			}

			//	element count as object
			size += serialize_binary(os, make_object<std::size_t>(v.size()));

			//
			//	serialize instruction to build a tuple
			//
			return size + serialize_binary(os, make_object(op::MAKE_TUPLE));
		}

		std::size_t serializer <vector_t, BINARY>::write(std::ostream& os, vector_t const& v)
		{
			std::size_t size{ 0 };

			//	serialize each element from vector
			for (auto const& obj : v) {
				size += serialize_binary(os, obj);
			}

			//	element count as object
			size += serialize_binary(os, make_object<std::size_t>(v.size()));

			//
			//	serialize instruction to build a vector
			//
			return size + serialize_binary(os, make_object(op::MAKE_VECTOR));

		}

		std::size_t serializer <deque_t, BINARY>::write(std::ostream& os, deque_t const& v)
		{
			std::size_t size{ 0 };

			//	serialize each element from set
			for (auto const& obj : v) {
				size += serialize_binary(os, obj);
			}

			//	element count as object
			size += serialize_binary(os, make_object<std::size_t>(v.size()));

			//
			//	serialize instruction to build a set
			//
			return size + serialize_binary(os, make_object(op::MAKE_DEQUE));
		}

	}
}
