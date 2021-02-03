/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/obj/object_cast.hpp>

namespace cyng {

	buffer_t to_buffer(object const& obj)
	{
		auto p = object_cast<buffer_t>(obj);
		return (p != nullptr)
			? *p
			: buffer_t{}
		;
	}
}

