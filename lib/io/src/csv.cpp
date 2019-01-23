/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/serializer/csv.hpp>
#include <cyng/io/serializer.h>
#include <cyng/io/io_buffer.h>
#include <cyng/object.h>
#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>

#include <cmath>
#include <algorithm>

#include <boost/io/ios_state.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng 
{	
	namespace io
	{		
		std::ostream& serializer<param_t, SERIALIZE_CSV>::write(std::ostream& os, param_t const& param)
		{
		
			os
				<< "\"cannot serialize "
				<< param.first
				<< '"'
				<< ' '
				;

			return os;
		}

		std::ostream& serializer<attr_t, SERIALIZE_CSV>::write(std::ostream& os, attr_t const& attr)
		{
			
			os
				<< "\"cannot serialize "
				<< attr.first
				<< '"'
				<< ':'
				<< ' '
				;

			return os;
		}

		std::ostream& serializer <tuple_t, SERIALIZE_CSV>::write(std::ostream& os, tuple_t const& v)
		{
			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj){
				if (!init)	{
					init = true;
				}
				else {
					os << ", ";
				}
				serialize_csv(os, obj);
			});

			return os << std::endl;
		}
		
		std::ostream& serializer <vector_t, SERIALIZE_CSV>::write(std::ostream& os, vector_t const& v)
		{
			//	serialize each element from the tuple
			bool init = false;
			std::size_t tag{ 0 };
			std::for_each(v.begin(), v.end(), [&os, &init, &tag](object const& obj){
				if (!init)	{
					tag = obj.get_class().tag();
					init = (tag != TC_TUPLE) && (tag != TC_VECTOR) && (tag != TC_SET);
				}
				else {
					os << ", ";
				}

				BOOST_ASSERT_MSG(obj.get_class().tag() == tag, "inconsistent vector");
				serialize_csv(os, obj);
	
			});

			return os;
		}

		std::ostream& serializer <set_t, SERIALIZE_CSV>::write(std::ostream& os, set_t const& v)
		{

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj){
				if (!init)	{
					init = true;
				}
				else {
					os << ", ";
				}
				serialize_csv(os, obj);
			});

			return os << std::endl;
		}
		
		std::ostream& serializer <attr_map_t, SERIALIZE_CSV>::write(std::ostream& os, attr_map_t const& v)
		{
			bool flag = false;
			std::for_each(v.begin(), v.end(), [&flag, &os](attr_map_t::value_type p) {

				if (flag)	{
					os << ',';
				}
				else {
					flag = true;
				}

				//
				//	ignore index
				//

				//
				//	serialize value
				//
				serialize_csv(os, p.second);
			});

			return os << std::endl;
		}

		std::ostream& serializer <param_map_t, SERIALIZE_CSV>::write(std::ostream& os, param_map_t const& v)
		{

			bool flag = false;
			std::for_each(v.begin(), v.end(), [&flag, &os](param_map_t::value_type p) {

				if (flag) 	{
					os << ',';
				}
				else
				{
					flag = true;
				}


				//
				//	ignore parameter name
				//

				//
				//	serialize value
				//
				serialize_csv(os, p.second);
			});

			return os << std::endl;
		}

		std::ostream& serializer <std::string, SERIALIZE_CSV>::write(std::ostream& os, std::string const& v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::setfill('0')
				<< std::hex
				<< '"'
				;

			//
			//	use UTF-8 iterator
			//
			auto first = v.begin();
			auto last = v.end();
			std::for_each(boost::u8_to_u32_iterator<std::string::const_iterator>(first), boost::u8_to_u32_iterator<std::string::const_iterator>(last), [&os](std::uint32_t c) {

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
					//
					//	single " characters are written as ""
					//
					os << '\"' << '\"';
					//os << '\\' << '\"';
					break;
				default:
					if (c > 30 && c < 127)
					{
						os << (char)c;
					}
					else
					{
						//	escape other non-printable characters
						os
							<< "\\u"
							<< std::setw(4)
							<< c
							;
					}
					break;
				}
			});

			os << '"';
			return os;		
		}
		
		std::ostream& serializer <bool, SERIALIZE_CSV>::write(std::ostream& os, bool v)
		{
			return os << ((v) ? "true" : "false");
		}
		
		std::ostream& serializer <code, SERIALIZE_CSV>::write(std::ostream& os, code c)
		{
			os 
			<< '"'
			<< c
			<< '"'
			;
			return os;
		}		
		
		std::ostream& serializer <boost::uuids::uuid, SERIALIZE_CSV>::write(std::ostream& os, boost::uuids::uuid v)
		{
			os 
			<< '"'
			<< v
			<< '"'
			;
			return os;
		}		
		
		std::ostream& serializer <buffer_t, SERIALIZE_CSV>::write(std::ostream& os, buffer_t const& v)
		{
			os 
				<< '"'
				<< to_hex(v)
				<< '"'
				;
			return os;
		}
		
		std::ostream& serializer <color_8, SERIALIZE_CSV>::write(std::ostream& os, color_8 const& v)
		{
			os 
				<< '"'
				<< "color-8"
				<< '"'
				;
			return os;
		}
		
		std::ostream& serializer <color_16, SERIALIZE_CSV>::write(std::ostream& os, color_16 const& v)
		{
			os 
				<< '"'
				<< "color-16"
				<< '"'
				;
			return os;
		}
		
		std::ostream& serializer <std::chrono::system_clock::time_point, SERIALIZE_CSV>::write(std::ostream& os, std::chrono::system_clock::time_point const& v)
		{
			os << '@' << to_str_iso(v);
			//os << '"' << v << '"';
			return os;
		}
		
		std::ostream& serializer <null, SERIALIZE_CSV>::write(std::ostream& os, null v)
		{
			os << "null";
			return os;
		}
		
		std::ostream& serializer <std::uint8_t, SERIALIZE_CSV>::write(std::ostream& os, std::uint8_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			os << std::dec << +v;
			return os;
		}

		std::ostream& serializer <std::int8_t, SERIALIZE_CSV>::write(std::ostream& os, std::int8_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			if ((v >= 0x20) && (v <= 0x7e)) {
				os
					<< '"'
					<< v
					<< '"'
					;
			}
			else {
				serializer<std::string, SERIALIZE_CSV>::write(os, std::string(1, v));
			}
			return os;
		}

		std::ostream& serializer <std::int16_t, SERIALIZE_CSV>::write(std::ostream& os, std::int16_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			return os << std::dec << v;
		}

		std::ostream& serializer <std::uint16_t, SERIALIZE_CSV>::write(std::ostream& os, std::uint16_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			return os << std::dec << v;
		}

		std::ostream& serializer <std::int32_t, SERIALIZE_CSV>::write(std::ostream& os, std::int32_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			return os << std::dec << v;
		}
		
		std::ostream& serializer <std::uint32_t, SERIALIZE_CSV>::write(std::ostream& os, std::uint32_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			return os << std::dec << v;
		}
		
		std::ostream& serializer <std::int64_t, SERIALIZE_CSV>::write(std::ostream& os, std::int64_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			return os << std::dec << v;
		}
		
		std::ostream& serializer <std::uint64_t, SERIALIZE_CSV>::write(std::ostream& os, std::uint64_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			return os << std::dec << v;
		}
		
		std::ostream& serializer <float, SERIALIZE_CSV>::write(std::ostream& os, float v)
		{
			if (std::isnan(v))
			{
				os << "null";
			}
			else 
			{
				boost::io::ios_flags_saver  ifs(os);
				os.precision(std::numeric_limits< float >::max_digits10);
				os << v;
			}
			return os;
		}

		std::ostream& serializer <double, SERIALIZE_CSV>::write(std::ostream& os, double v)
		{
			if (std::isnan(v))
			{
				os << "null";
			}
			else 
			{
				boost::io::ios_flags_saver  ifs(os);
				os.precision(std::numeric_limits< double >::max_digits10);
				os << v;
			}
			return os;
		}
	}
}


