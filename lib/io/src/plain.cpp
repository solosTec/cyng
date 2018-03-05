/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/serializer/plain.hpp>
#include <cyng/io/serializer.h>
#include <cyng/object.h>
#include <algorithm>
#include <boost/io/ios_state.hpp>

namespace cyng 
{	
	namespace io
	{
		
		std::ostream& serializer <tuple_t, SERIALIZE_PLAIN>::write(std::ostream& os, tuple_t const& v)
		{
			os << '{';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj){
				if (!init)	{
					init = true;
				}
				else {
					os << ",";
				}
				serialize_plain(os, obj);
			});

			os << '}';
			return os;
			
		}
		
		std::ostream& serializer <vector_t, SERIALIZE_PLAIN>::write(std::ostream& os, vector_t const& v)
		{
			os << '[';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj){
				if (!init)	{
					init = true;
				}
				else {
					os << ",";
				}
				serialize_plain(os, obj);
			});

			os << ']';
			return os;
			
		}

		std::ostream& serializer <set_t, SERIALIZE_PLAIN>::write(std::ostream& os, set_t const& v)
		{
			os << '<';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj){
				if (!init)	{
					init = true;
				}
				else {
					os << ",";
				}
				serialize_plain(os, obj);
			});

			os << '>';
			return os;		
		}
		
		std::ostream& serializer <attr_map_t, SERIALIZE_PLAIN>::write(std::ostream& os, attr_map_t const& v)
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
				serialize_plain(os, attr.second);
				os << ')';
			});
			os << ')';
			return os;
		}

		std::ostream& serializer <param_map_t, SERIALIZE_PLAIN>::write(std::ostream& os, param_map_t const& v)
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
				serialize_plain(os, param.second);
				os << ')';
			});
			os << ')';
			return os;
		}

		std::ostream& serializer <color_8, SERIALIZE_PLAIN>::write(std::ostream& os, color_8 const& v)
		{
			os << "color-8";
			return os;
		}
		
		std::ostream& serializer <color_16, SERIALIZE_PLAIN>::write(std::ostream& os, color_16 const& v)
		{
			os << "color-16";
			return os;
		}
		
		std::ostream& serializer <bool, SERIALIZE_PLAIN>::write(std::ostream& os, bool v)
		{
			boost::io::ios_flags_saver  ifs(os);
			os << std::boolalpha << v;
			return os;
		}
		
		std::ostream& serializer <buffer_t, SERIALIZE_PLAIN>::write(std::ostream& os, buffer_t const& v)
		{
			constexpr char hexa[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

			for (const char c : v)
			{
				os 
					<< hexa[(c & 0xf0) >> 4] 
					<< hexa[(c & 0x0f)]
					;
			}
			return os;
		}
		
		std::ostream& serializer <boost::uuids::uuid, SERIALIZE_PLAIN>::write(std::ostream& os, boost::uuids::uuid const& v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);
	
			os
				<< std::hex
				<< std::setfill('0')
				;
	
			//
			// 	format xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
			//	36 characters
	
			std::size_t pos = 0;
			std::for_each(v.begin(), v.end(), [&os, &pos](char c) {
				os
					<< std::setw(2)
					<< (+c & 0xFF)	//	promote to integer
					;
	
				++pos;
				if (pos == 4 || pos == 6 || pos == 8 || pos == 10) {
					os << '-';
				}
	
			});
	
			return os;
		}
		
		std::ostream& serializer <std::string, SERIALIZE_PLAIN>::write(std::ostream& os, std::string const& v)
		{
			for (char c : v)
			{
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
					os << c;
					//	ToDo: escape other non-printable characters
					break;
				}
			}
			return os;
		}

		std::ostream& serializer <std::uint8_t, SERIALIZE_PLAIN>::write(std::ostream& os, std::uint8_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::hex
				<< std::setfill('0')
				<< +v
				;
			return os;
		}
		std::ostream& serializer <std::uint16_t, SERIALIZE_PLAIN>::write(std::ostream& os, std::uint16_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::hex
				<< std::setfill('0')
				<< std::setw(4)
				<< v
				;
			return os;
		}
		std::ostream& serializer <std::uint32_t, SERIALIZE_PLAIN>::write(std::ostream& os, std::uint32_t v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::hex
				<< std::setfill('0')
				<< std::setw(8)
				<< v
				;
			return os;
		}

		std::ostream& serializer_custom<SERIALIZE_PLAIN>::write(std::ostream& os, std::size_t tag, std::string const& type_name, object const& obj)
		{
			os
				<< "<!"
				<< tag
				<< ':'
				<< type_name
				<< '>'
				;
			return os;

		}
	}
}

