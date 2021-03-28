/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serializer/json.hpp>
#include <cyng/io/serializer/plain.hpp>
#include <cyng/io/serialize.h>
#include <cyng/obj/tag.hpp>
#include <cyng/io/ostream.h>
#include <cyng/io/parser/utf-8.h>

#include  <iomanip>

#include <boost/io/ios_state.hpp>

namespace cyng {
	namespace io {

		std::size_t serializer <bool, JSON>::write(std::ostream& os, bool v)
		{
			return serializer<bool, PLAIN>::write(os, v);
		}

		std::size_t serializer <null, JSON>::write(std::ostream& os, null n)
		{
			auto const pos = os.tellp();
			os << n;
			return os.tellp() - pos;
		}

		std::size_t serializer <vector_t, JSON>::write(std::ostream& os, vector_t const& vec)
		{
			auto const pos = os.tellp();
			if (vec.empty()) {
				os << "null";
			}
			else {
				os << '[';

				//	serialize each element from the tuple
				bool init = false;

				for (auto const& obj : vec) {
					if (!init) {
						init = true;
					}
					else {
						os << ", ";
					}
					auto const tag = obj.rtti().tag();
					switch (tag)
					{
					case TC_ATTR:
					case TC_PARAM:
						os << '{';
						serialize_json(os, obj);
						os << '}';
						break;
					default:
						serialize_json(os, obj);
					}
				}

				os << ']';
			}
			return os.tellp() - pos;

		}

		std::size_t serializer <tuple_t, JSON>::write(std::ostream& os, tuple_t const& tpl) {

			auto const pos = os.tellp();
			if (tpl.empty()) {
				os << "null";
			}
			else {
				os << '{';

				//	serialize each element from the tuple
				bool init = false;
				for (auto const& obj : tpl) {
					if (!init) {
						init = true;
					}
					else {
						os << ", ";
					}
					serialize_json(os, obj);
				}

				os << '}';
			}
			return os.tellp() - pos;
		}

		std::size_t serializer <param_t, JSON>::write(std::ostream& os, param_t const& param) {

			auto const pos = os.tellp();
			os
				<< '"'
				<< param.first
				<< '"'
				<< ':'
				<< ' '
				;

			auto const tag = param.second.rtti().tag();
			switch (tag)
			{
			case TC_ATTR:
			case TC_PARAM:
				os << '{';
				serialize_json(os, param.second);
				os << '}';
				break;
			default:
				serialize_json(os, param.second);
			}

			return os.tellp() - pos;
		}

		std::size_t serializer <param_map_t, JSON>::write(std::ostream& os, param_map_t const& pm) {

			auto const pos = os.tellp();
			if (pm.empty()) {
				os << "null";
			}
			else {
				os << '{';

				bool flag = false;
				for (auto const& p : pm) {
					if (flag) {
						os << ',';
					}
					else {
						flag = true;
					}

					os
						<< '"'
						<< p.first
						<< '"'
						<< ':'
						;

					serialize_json(os, p.second);
				}
				os << '}';
			}
			return os.tellp() - pos;
		}


		std::size_t serializer <std::int8_t, JSON>::write(std::ostream& os, std::int8_t v)
		{
			return serializer<std::int8_t, PLAIN>::write(os, v);
		}
		std::size_t serializer <std::int16_t, JSON>::write(std::ostream& os, std::int16_t v)
		{
			return serializer<std::int16_t, PLAIN>::write(os, v);
		}
		std::size_t serializer <std::int32_t, JSON>::write(std::ostream& os, std::int32_t v)
		{
			return serializer<std::int32_t, PLAIN>::write(os, v);
		}
		std::size_t serializer <std::int64_t, JSON>::write(std::ostream& os, std::int64_t v)
		{
			return serializer< std::int64_t, PLAIN>::write(os, v);
		}
		std::size_t serializer <std::uint8_t, JSON>::write(std::ostream& os, std::uint8_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			auto const pos = os.tellp();

			os
				<< std::dec
				<< +v
				;
			return os.tellp() - pos;
		}
		std::size_t serializer <std::uint16_t, JSON>::write(std::ostream& os, std::uint16_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			auto const pos = os.tellp();

			os
				<< std::dec
				<< v
				;
			return os.tellp() - pos;
		}
		std::size_t serializer <std::uint32_t, JSON>::write(std::ostream& os, std::uint32_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			auto const pos = os.tellp();

			os
				<< std::dec
				<< v
				;
			return os.tellp() - pos;
		}
		std::size_t serializer <std::uint64_t, JSON>::write(std::ostream& os, std::uint64_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			auto const pos = os.tellp();

			os
				<< std::dec
				<< v
				;
			return os.tellp() - pos;
		}
		std::size_t serializer <double, JSON>::write(std::ostream& os, double v)
		{
			return serializer<double, PLAIN>::write(os, v);
		}
		std::size_t serializer <std::string, JSON>::write(std::ostream& os, std::string const& str)
		{
			auto const pos = os.tellp();
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::setfill('0')
				<< std::hex
				<< '"'
				;

			utf8::u8_to_u32_iterator begin(str.begin());
			utf8::u8_to_u32_iterator end(str.end());
			for (; begin != end; ++begin) {
				auto const c = *begin;

				switch (c)
				{
				case '\a':
					os << '\\' << 'a';
					break;
				case '\b':
					os << '\\' << 'b';
					break;
				case '\f':
					os << '\\' << 'f';
					break;
				case '\n':
					os << '\\' << 'n';
					break;
				case '\r':
					os << '\\' << 'r';
					break;
				case '\t':
					os << '\\' << 't';
					break;
				case '\v':
					os << '\\' << 'v';
					break;
				case '\\':
					os << '\\' << '\\';
					break;
				case '\'':
					os << '\\' << '\'';
					break;
				case '\"':
					os << '\\' << '\"';
					break;
				default:
					if (c > 30 && c < 127)	{
						os << (char)c;
					}
					else	{
						//	escape other non-printable characters
						os
							<< "\\u"
							<< std::setw(4)
							<< std::setfill('0')
							<< c
							;
					}
					break;
				}
			}

			os << '"';

			return os.tellp() - pos;
		}

		std::size_t serializer <buffer_t, JSON>::write(std::ostream& os, buffer_t const& buffer)
		{
			auto const pos = os.tellp();
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
			os << '"' << buffer << '"';
			return os.tellp() - pos;
		}

		std::size_t serializer <std::chrono::system_clock::time_point, JSON>::write(std::ostream& os, std::chrono::system_clock::time_point const& v)	{

			auto const pos = os.tellp();
			std::time_t const tt = std::chrono::system_clock::to_time_t(v);
			auto tm = *std::gmtime(&tt);
			//	example: 1995-12-04T00:12:00-0430
			// ISO 8601
			os << '"' << std::put_time(&tm, "%FT%T%z") << '"';

			return os.tellp() - pos;

		}

		std::size_t serializer <std::filesystem::path, JSON>::write(std::ostream& os, std::filesystem::path const& v)	{

			auto const pos = os.tellp();
			os << '"' << v.string() << '"';
			return os.tellp() - pos;
		}

	}
}
