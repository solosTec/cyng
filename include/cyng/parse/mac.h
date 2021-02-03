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
	mac48 to_mac48(std::string const& str);
}
#endif

