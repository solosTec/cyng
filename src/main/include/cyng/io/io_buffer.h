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
		std::string to_ascii(buffer_t const&);

		/**
		 * Writes a binary buffer as hex values
		 */
		std::string to_hex(buffer_t const&);

		/**
		 * Writes a binary buffer as hex values separated
		 * by a single character (preferrable a white space)
		 */
		std::string to_hex(buffer_t const&, char);
	}
}

#endif // CYNG_IO_BUFFER_H
