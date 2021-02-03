/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_JSON_HPP
#define CYNG_IO_JSON_HPP

#include <cyng/io/io.h>

namespace cyng {
	namespace io {

		template <>
		struct serializer <bool, JSON>
		{
			static std::size_t write(std::ostream& os, bool v);
		};
	}
}
#endif
