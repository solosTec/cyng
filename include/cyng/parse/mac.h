/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_MAC_H
#define CYNG_PARSE_MAC_H


#include <cyng/obj/intrinsics/mac.h>
#include <string>

namespace cyng {
	/**
	 * expected format is "00:0c:29:cc:e3:d4"
	 */
	mac48 to_mac48(std::string const& str);

	/**
	 * expected format is "70B3:D538:7000:002D"
	 */
	mac64 to_mac64(std::string const& str);
}
#endif

