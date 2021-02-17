/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_DOM_TRANSFORMER_H
#define CYNG_OBJ_ALGORITHM_DOM_TRANSFORMER_H

#include <cyng/obj/object.h>
#include <cyng/obj/tag.hpp>

namespace cyng {

	/**
	 * walk down the object tree and apply tranformations
	 */
	void transform(object&, std::function<object(object const&, std::size_t, type_code)>);

}

#endif 
