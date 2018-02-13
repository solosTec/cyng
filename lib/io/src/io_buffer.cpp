/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/io/io_buffer.h>
#include <sstream>
#include <iomanip>
#include <boost/io/ios_state.hpp>

namespace cyng 
{	
	namespace io
	{
		std::ostream& operator<<(std::ostream& os, buffer_t const& v)
		{
			//	store and reset stream state
			boost::io::ios_flags_saver  ifs(os);

			os
				<< std::setfill('0')
				<< std::hex
				;

			//	write buffer
			for (const char c : v)
			{
				if (c > 31 && c < 127)
				{
					os << c;
				}
				else if (c == '\\')
				{
					os << "\\\\";
				}
				else if (c == '"')
				{
					os << "\\\"";
				}
				else if (c == '\n')
				{
					os << "\\n";
				}
				else if (c == '\t')
				{
					os << "\\t";
				}
				else
				{
					os
						<< "\\x"
						<< std::setw(2)
						<< (+c & 0xFF)
						;
				}
			}
			return os;
		}

		std::string to_ascii(buffer_t const& buffer)
		{
			std::stringstream ss;

			//	write buffer
			for (const char c : buffer)
			{
				if (c > 31 && c < 127)
				{
					ss << c;
				}
				else
				{
					ss << '.';
				}
			}
			return ss.str();
		}

		std::string to_hex(buffer_t const& buffer)
		{
			std::stringstream ss;
			ss
				<< std::setfill('0')
				<< std::hex
				;

			//	write buffer
			for (const char c : buffer)
			{
				ss
					<< std::setw(2)
					<< (+c & 0xFF)
					;
			}

			return ss.str();
		}

		std::string to_hex(buffer_t const& buffer, char sp)
		{
			std::stringstream ss;
			ss
				<< std::setfill('0')
				<< std::hex
				;

			//	write buffer
			bool flag = false;
			for (const char c : buffer)
			{
				if (flag)
				{
					ss << sp;
				}
				else
				{
					flag = true;
				}
				ss
					<< std::setw(2)
					<< (+c & 0xFF)
					;
			}

			return ss.str();
		}
	}
}

