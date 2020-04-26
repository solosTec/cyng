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
			return to_hex(os, buffer.begin(), buffer.end());
		}

		std::string to_hex(buffer_t const& buffer)
		{
			std::stringstream ss;
			to_hex(ss, buffer);
			return ss.str();
		}

		std::ostream& to_hex(std::ostream& os, buffer_t const& buffer, char sp)
		{
			boost::io::ios_flags_saver  ifs(os);
			os
				<< std::setfill('0')
				<< std::hex
				;

			//	write buffer
			bool flag = false;
			for (const char c : buffer)
			{
				if (flag)	{
					os << sp;
				}
				else {
					flag = true;
				}
				os
					<< std::setw(2)
					<< (+c & 0xFF)
					;
			}
			return os;
		}

		std::string to_hex(buffer_t const& buffer, char sp)
		{
			std::stringstream ss;
			to_hex(ss, buffer, sp);
			return ss.str();
		}

		std::ostream& to_hex(std::ostream& os, buffer_t::const_iterator pos, buffer_t::const_iterator end)
		{
			boost::io::ios_flags_saver  ifs(os);
			os
				<< std::setfill('0')
				<< std::hex
				;

			//	write buffer
			while (pos != end)
			{
				os
					<< std::setw(2)
					<< (*pos & 0xFF)
					;

				++pos;
			}
			return os;

		}

		std::string to_hex(buffer_t::const_iterator start, buffer_t::const_iterator end)
		{
			std::stringstream ss;
			to_hex(ss, start, end);
			return ss.str();
		}

	}
}

