/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include <cyng/buffer_cast.h>
#include <cyng/value_cast.hpp>

namespace cyng 
{
	buffer_t to_buffer(object obj)
	{
		buffer_t buffer;
		return cyng::value_cast<>(obj, buffer);
	}
}





