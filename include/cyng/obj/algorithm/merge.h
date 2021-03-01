/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_MERGE_H
#define CYNG_OBJ_ALGORITHM_MERGE_H

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/container.h>

#include <initializer_list>

namespace cyng {

	/**
	 * nested parameter maps
	 */
	void merge(param_map_t& pm, std::initializer_list<std::string> il, object const& val);
}

#endif 
