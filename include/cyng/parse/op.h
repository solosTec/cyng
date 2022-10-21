/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_OP_H
#define CYNG_PARSE_OP_H

#include <cyng/obj/intrinsics/op.h>
#include <string>
#include <boost/predef.h>

namespace cyng {
	
	op to_op(std::string const& str);
}
#endif

