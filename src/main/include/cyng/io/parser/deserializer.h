/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_IO_DESERIALIZER_H
#define CYNG_IO_DESERIALIZER_H


#include <cyng/object.h>
#include <istream>

namespace cyng 
{
	namespace io
	{
		object deserialize(std::istream&, std::size_t tag, std::uint64_t size);
	}
}

#endif 	//	CYNG_IO_DESERIALIZER_H

