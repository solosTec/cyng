/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_BUFFER_CAST_H
#define CYNG_BUFFER_CAST_H

#include <cyng/intrinsics/buffer.h>
#include <cyng/object.h>

namespace cyng 
{

	/**
	 * Convinient value_cast<buffer_t>()
     * Not suited for large buffer sizes.
	 */
	buffer_t to_buffer(object obj);

}

#endif 

