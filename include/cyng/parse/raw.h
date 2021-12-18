/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_PARSE_RAW_H
#define CYNG_PARSE_RAW_H


#include <cyng/obj/intrinsics/raw.h>
#include <cyng/obj/object.h>

namespace cyng {

	/**
	 * Create a 'real' object from the raw (unparsed) data.
	 * Note: This is not possible for every type.
	 */
	object make_object(raw const&);

}
#endif

