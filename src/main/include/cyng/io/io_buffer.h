/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_BUFFER_H
#define CYNG_IO_BUFFER_H


#include <cyng/io.h>
#include <cyng/intrinsics/buffer.h>
#include <iostream>

namespace cyng 
{	
	namespace io
	{
		/**
		 * Serialize a buffer with escaped non-printable ASCII characters
		 */
		std::ostream& operator<<(std::ostream& os, buffer_t const&);

		/**
		 * Writes a binary buffer as ASCII values and not printable characters as dot
		 */
		std::ostream& to_ascii(std::ostream&, buffer_t const&);
		std::string to_ascii(buffer_t const&);

		/**
		 * Writes a binary buffer as hex values without spaces
		 */
		std::ostream& to_hex(std::ostream&, buffer_t const&);
		std::string to_hex(buffer_t const&);

		/**
		 * Writes a binary buffer as hex values separated
		 * by a single character (preferrable a white space)
		 */
		std::ostream& to_hex(std::ostream&, buffer_t const&, char);
		std::string to_hex(buffer_t const&, char);

		/**
		 * faster and lighter int to hex conversion compared to streams.
		 * @see https://stackoverflow.com/a/33447587/3979819
		 */
		template <typename T> 
		std::string to_hex(T w, size_t length = sizeof(T) << 1) 
		{
			static const char digits[] = "0123456789ABCDEF";
			std::string str(length, '0');
			for (size_t idx = 0, pos = (length - 1) * 4; idx < length; ++idx, pos -= 4) {
				str[idx] = digits[(w >> pos) & 0x0f];
			}
			return str;
		}
	}
}

#endif // CYNG_IO_BUFFER_H
