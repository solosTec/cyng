/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/serializer/json.hpp>
#include <cyng/io/serializer.h>
#include <cyng/object.h>
#include <cmath>
#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <algorithm>
#include <boost/io/ios_state.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/regex/pending/unicode_iterator.hpp>

namespace cyng 
{	
	namespace io
	{
		
		
		std::ostream& serializer<param_t, SERIALIZE_JSON>::write(std::ostream& os, param_t const& param)
		{
		
			os
			<< '"'
			<< param.first
			<< '"'
			<< ':'
			<< ' '
			;

			serialize_json(os, param.second);

			return os;
		}

		std::ostream& serializer<attr_t, SERIALIZE_JSON>::write(std::ostream& os, attr_t const& attr)
		{
			
			os
			<< '"'
			<< attr.first
			<< '"'
			<< ':'
			<< ' '
			;

			serialize_json(os, attr.second);

			return os;
		}

		std::ostream& serializer <tuple_t, SERIALIZE_JSON>::write(std::ostream& os, tuple_t const& v)
		{
			os << '{';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj){
				if (!init)	{
					init = true;
				}
				else {
					os << ", ";
				}
				serialize_json(os, obj);
			});

			os << '}';
			return os;
			
		}
		
		std::ostream& serializer <vector_t, SERIALIZE_JSON>::write(std::ostream& os, vector_t const& v)
		{
			os << '[';

			//	serialize each element from the tuple
			bool init = false;
			std::size_t tag{ 0 };
			std::for_each(v.begin(), v.end(), [&os, &init, &tag](object const& obj){
				if (!init)	{
					init = true;
					tag = obj.get_class().tag();
				}
				else {
					os << ", ";
				}
				BOOST_ASSERT_MSG(obj.get_class().tag() == tag, "inconsistent vector");
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
	
			});
			os << ']';
			return os;
			
		}

		std::ostream& serializer <set_t, SERIALIZE_JSON>::write(std::ostream& os, set_t const& v)
		{
			os << '[';

			//	serialize each element from the tuple
			bool init = false;
			std::for_each(v.begin(), v.end(), [&os, &init](object const& obj){
				if (!init)	{
					init = true;
				}
				else {
					os << ", ";
				}
				serialize_json(os, obj);
			});

			os << ']';
			return os;		
		}
		
		std::ostream& serializer <attr_map_t, SERIALIZE_JSON>::write(std::ostream& os, attr_map_t const& v)
		{
			os << '{';

			bool flag = false;
			std::for_each(v.begin(), v.end(), [&flag, &os](attr_map_t::value_type p) {

				if (flag)
				{
					os << ',';
				}
				else
				{
					flag = true;
				}

				os
					<< '"'
					<< p.first
					<< '"'
					<< ':'
					;

				serialize_json(os, p.second);
			});

			os << '}';

			return os;
		}

		std::ostream& serializer <param_map_t, SERIALIZE_JSON>::write(std::ostream& os, param_map_t const& v)
		{
			os << '{';

			bool flag = false;
			std::for_each(v.begin(), v.end(), [&flag, &os](param_map_t::value_type p) {

				if (flag)
				{
					os << ',';
				}
				else
				{
					flag = true;
				}

				os
					<< '"'
					<< p.first
					<< '"'
					<< ':'
					;

				serialize_json(os, p.second);
			});

			os << '}';

			return os;
		}

		std::ostream& serializer <std::string, SERIALIZE_JSON>::write(std::ostream& os, std::string const& v)
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
					os << '\\' << '\"';
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
		
		std::ostream& serializer <bool, SERIALIZE_JSON>::write(std::ostream& os, bool v)
		{
			if (v)	os << "true";
			else os << "false";
			return os;		
		}
		
		std::ostream& serializer <code, SERIALIZE_JSON>::write(std::ostream& os, code c)
		{
			os 
			<< '"'
			<< c
			<< '"'
			;
			return os;
		}		
		
		std::ostream& serializer <boost::uuids::uuid, SERIALIZE_JSON>::write(std::ostream& os, boost::uuids::uuid v)
		{
			os 
			<< '"'
			<< v
			<< '"'
// 			<< ":UUID"
			;
			return os;
		}		
		
		std::ostream& serializer <buffer_t, SERIALIZE_JSON>::write(std::ostream& os, buffer_t const& v)
		{
			os 
			<< "ToDo: buffer_t"
			;
			return os;
		}
		
		std::ostream& serializer <color_8, SERIALIZE_JSON>::write(std::ostream& os, color_8 const& v)
		{
			os << "color-8";
			return os;
		}
		
		std::ostream& serializer <color_16, SERIALIZE_JSON>::write(std::ostream& os, color_16 const& v)
		{
			os << "color-16";
			return os;
		}
		
		std::ostream& serializer <std::chrono::system_clock::time_point, SERIALIZE_JSON>::write(std::ostream& os, std::chrono::system_clock::time_point const& v)
		{
			os << '"' << v << '"';
			return os;
		}
		
		std::ostream& serializer <null, SERIALIZE_JSON>::write(std::ostream& os, null v)
		{
			os << "null";
			return os;
		}
		
		std::ostream& serializer <std::uint8_t, SERIALIZE_JSON>::write(std::ostream& os, std::uint8_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			os << std::dec << +v;
			return os;
		}

		std::ostream& serializer <std::int32_t, SERIALIZE_JSON>::write(std::ostream& os, std::int32_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			os << std::dec << v;
			return os;
		}
		
		std::ostream& serializer <std::uint32_t, SERIALIZE_JSON>::write(std::ostream& os, std::uint32_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			os << std::dec << v;
			return os;
		}
		
		std::ostream& serializer <std::int64_t, SERIALIZE_JSON>::write(std::ostream& os, std::int64_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			os << std::dec << v;
			return os;
		}
		
		std::ostream& serializer <std::uint64_t, SERIALIZE_JSON>::write(std::ostream& os, std::uint64_t v)
		{
			boost::io::ios_flags_saver  ifs(os);
			os << std::dec << v;
			return os;
		}
		
		std::ostream& serializer <float, SERIALIZE_JSON>::write(std::ostream& os, float v)
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

		std::ostream& serializer <double, SERIALIZE_JSON>::write(std::ostream& os, double v)
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


