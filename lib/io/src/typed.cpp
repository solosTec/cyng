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
				case '\n':
					os << "\\n";
					break;
				case '\t':
					os << "\\t";
					break;
				case '\a':
					os << "\\a";
					break;
				case '\f':
					os << "\\f";
					break;
				case '"':
					os << "\"";
					break;
				case '\\':
					os << "\\\\";
					break;
				default:
					if ((c > 31) && (c < 127))
					{
						os << c;
					}
					else
					{
						os
							<< "\\u"
							<< std::hex
							<< std::setw(4)
							<< +c
							;
					}
					break;
				}
			});
			os << '"';
			return os;
		}
	}
}


