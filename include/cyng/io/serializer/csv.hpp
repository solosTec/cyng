/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_IO_CSV_HPP
#define DOCC_IO_CSV_HPP

#include <cyng/io/io.h>

namespace docscript {
	namespace io {

		template <>
		struct serializer <bool, CSV>
		{
			static std::size_t write(std::ostream& os, bool v);
		};

	}
}
#endif
