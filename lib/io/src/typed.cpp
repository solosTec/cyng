/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/serializer/typed.hpp>
#include <cyng/io/serializer.h>
#include <cyng/object.h>
#include <boost/io/ios_state.hpp>

namespace cyng 
{	
	namespace io
	{
		
		std::ostream& serializer <version, SERIALIZE_TYPED>::write(std::ostream& os, version const& v)
		{
			os << cyng::traits::get_tag_name<version>();
			serializer<version, SERIALIZE_PLAIN>::write(os, v);
			return os;		
		}
		
		std::ostream& serializer <revision, SERIALIZE_TYPED>::write(std::ostream& os, revision const& v)
		{
			os << cyng::traits::get_tag_name<revision>();
			serializer<revision, SERIALIZE_PLAIN>::write(os, v);
			return os;		
		}
		
		std::ostream& serializer <std::chrono::system_clock::time_point, SERIALIZE_TYPED>::write(std::ostream& os, std::chrono::system_clock::time_point v)
		{
			os << '"';
			serializer<std::chrono::system_clock::time_point, SERIALIZE_PLAIN>::write(os, v);
			os << '"' << cyng::traits::get_tag_name<std::chrono::system_clock::time_point>();
			return os;
		}
		
		std::ostream& serializer <std::string, SERIALIZE_TYPED>::write(std::ostream& os, std::string const& v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os << '"';
			std::for_each(v.begin(), v.end(), [&os](char c) {
				switch (c)
				{
				case '\a':	os << "\\a";	break;
				case '\b':	os << "\\b";	break;
				case '\f':	os << "\\f";	break;
				case '\n':	os << "\\n";	break;
				case '\r':	os << "\\r";	break;
				case '\t':	os << "\\t";	break;
				case '\v':	os << "\\v";	break;
				case '\\':	os << "\\\\";	break;
				case '\'':	os << "\\\'";	break;
				case '"':	os << "\"";		break;
				default:
					if ((c > 31) && (c < 127))	{
						os << c;
					}
					else	{
						os
							<< "\\u"
							<< std::hex
							<< std::setw(2)
							<< +c
							;
					}
					break;
				}
			});
			os << '"';
			return os;
		}

		std::ostream& serializer <boost::asio::ip::tcp::endpoint, SERIALIZE_TYPED>::write(std::ostream& os, boost::asio::ip::tcp::endpoint const& v)
		{
			auto const address = v.address().to_string();
			auto const port = v.port();

			//
			//	"address:port"protocolType
			//
			return os
				<< '"'
				<< address
				<< ':'
				<< port
				<< '"'
				<< (v.address().is_v4() ? "IPv4" : "IPv6")
				;

		}

		std::ostream& serializer<tuple_t, SERIALIZE_TYPED>::write(std::ostream& os, tuple_t const& v)
		{
			os << '{';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj) {
				if (!init) {
					init = true;
				}
				else {
					os << ",";
				}
				serialize_typed(os, obj);
			});

			os << '}';
			return os;
		}

		std::ostream& serializer <vector_t, SERIALIZE_TYPED>::write(std::ostream& os, vector_t const& v)
		{
			os << '[';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj) {
				if (!init) {
					init = true;
				}
				else {
					os << ",";
				}
				serialize_typed(os, obj);
			});

			os << ']';
			return os;
		}

		std::ostream& serializer <attr_map_t, SERIALIZE_TYPED>::write(std::ostream& os, attr_map_t const& v)
		{
			os << '#' << '(';
			bool flag = false;
			std::for_each(v.begin()
				, v.end()
				, [&flag, &os](attr_map_t::value_type const& attr) {

					if (flag)
					{
						os << ',';
					}
					else
					{
						flag = true;
					}

					os << '(' << attr.first << ':';
					serialize_typed(os, attr.second);
					os << ')';
				});
			os << ')';
			return os;
		}

		std::ostream& serializer <param_map_t, SERIALIZE_TYPED>::write(std::ostream& os, param_map_t const& v)
		{
			bool flag = false;
			os << '%' << '(';
			std::for_each(v.begin()
				, v.end()
				, [&flag, &os](param_map_t::value_type const& param) {

					if (flag)
					{
						os << ',';
					}
					else
					{
						flag = true;
					}

					os << '(' << '"' << param.first << '"' << ':';
					serialize_typed(os, param.second);
					os << ')';
				});
			os << ')';
			return os;
		}

		std::ostream& serializer <set_t, SERIALIZE_TYPED>::write(std::ostream& os, set_t const& v)
		{
			os << '<';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj) {
				if (!init) {
					init = true;
				}
				else {
					os << ",";
				}
				serialize_typed(os, obj);
			});

			os << '>';
			return os;
		}

		std::ostream& serializer <color_8, SERIALIZE_TYPED>::write(std::ostream& os, color_8 const& v)
		{
			os << "color-8";
			return os;
		}

		std::ostream& serializer <color_16, SERIALIZE_TYPED>::write(std::ostream& os, color_16 const& v)
		{
			os << "color-16";
			return os;
		}

		std::ostream& serializer <std::int8_t, SERIALIZE_TYPED>::write(std::ostream& os, std::int8_t v)
		{
			serializer<std::int8_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::int8_t>();
			return os;
		}

		std::ostream& serializer <std::uint8_t, SERIALIZE_TYPED>::write(std::ostream& os, std::uint8_t v)
		{
			serializer<std::uint8_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::uint8_t>();
			return os;
		}

		std::ostream& serializer <std::int16_t, SERIALIZE_TYPED>::write(std::ostream& os, std::uint16_t v)
		{
			serializer<std::int16_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::int16_t>();
			return os;
		}

		std::ostream& serializer <std::uint16_t, SERIALIZE_TYPED>::write(std::ostream& os, std::uint16_t v)
		{
			serializer<std::uint16_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::uint16_t>();
			return os;
		}

		std::ostream& serializer <std::int32_t, SERIALIZE_TYPED>::write(std::ostream& os, std::int32_t v)
		{
			serializer<std::int32_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::int32_t>();
			return os;
		}

		std::ostream& serializer <std::uint32_t, SERIALIZE_TYPED>::write(std::ostream& os, std::uint32_t v)
		{
			serializer<std::uint32_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::uint32_t>();
			return os;
		}

		std::ostream& serializer <std::int64_t, SERIALIZE_TYPED>::write(std::ostream& os, std::int64_t v)
		{
			serializer<std::int64_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::int64_t>();
			return os;
		}

		std::ostream& serializer <std::uint64_t, SERIALIZE_TYPED>::write(std::ostream& os, std::uint64_t v)
		{
			serializer<std::uint64_t, SERIALIZE_PLAIN>::write(os, v);
			os << cyng::traits::get_tag_name<std::uint64_t>();
			return os;
		}

		std::ostream& serializer <buffer_t, SERIALIZE_TYPED>::write(std::ostream& os, buffer_t const& v)
		{
			os << "'";
			serializer<buffer_t, SERIALIZE_PLAIN>::write(os, v);
			os << "'";
			return os;
		}

	}
}


