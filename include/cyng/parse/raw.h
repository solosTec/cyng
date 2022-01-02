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
	 * This is different then make_object() so we need a different name - hence construct_object()
	 */
	object construct_object(raw const&);

	/**
	 * @return true if specified data type is constructable (from raw type)
	 */
	bool is_constructable(std::uint16_t tc);


}
#endif

