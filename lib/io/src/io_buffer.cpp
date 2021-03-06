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

		std::ostream& to_ascii(std::ostream& os, buffer_t const& buffer)
		{
			for (const char c : buffer)
			{
				if ((c > 31) && (c < 127))	{
					os << c;
				}
				else	{
					os << '.';
				}
			}
			return os;
		}

		std::string to_ascii(buffer_t const& buffer)
		{
			std::stringstream ss;
			to_ascii(ss, buffer);
			//	write buffer
			return ss.str();
		}

		std::ostream& to_hex(std::ostream& os, buffer_t const& buffer)
		{
			boost::io::ios_flags_saver  ifs(os);
			os
				<< std::setfill('0')
				<< std::hex
				;

			//	write buffer
			for (const char c : buffer)
			{
				os
					<< std::setw(2)
					<< (+c & 0xFF)
					;
			}
			return os;
		}

		std::string to_hex(buffer_t const& buffer)
		{
			std::stringstream ss;
			to_hex(ss, buffer);
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

