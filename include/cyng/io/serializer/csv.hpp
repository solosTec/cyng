/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_CSV_HPP
#define CYNG_IO_CSV_HPP

#include <cyng/io/io.h>

#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/buffer.h>
#include <cyng/obj/intrinsics/null.h>
#include <cyng/obj/intrinsics/severity.h>

namespace cyng {
	namespace io {

		template <>
		struct serializer <bool, CSV>
		{
			static std::size_t write(std::ostream& os, bool v);
		};

		template <>
		struct serializer <vector_t, CSV>
		{
			static std::size_t write(std::ostream& os, vector_t const&);
		};
		template <>
		struct serializer <tuple_t, CSV>
		{
			static std::size_t write(std::ostream& os, tuple_t const&);
		};

	}
}
#endif
