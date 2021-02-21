/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_BUFFER_H
#define CYNG_PARSE_BUFFER_H

#include <cyng/obj/intrinsics/buffer.h>
#include <string>

namespace cyng {

	/**
	 * Expect an string with hexadecimal characters and
	 * an even numbered length.
	 */
	buffer_t to_buffer(std::string const&);
	
}
#endif

