/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_IO_BUFFER_H
#define CYNG_IO_BUFFER_H

#include <cyng/obj/intrinsics/buffer.h>

#include <ostream>
#include <type_traits>

namespace cyng {
	namespace io {

		/**
		 * faster and lighter int to hex conversion compared to streams.
		 * @see https://stackoverflow.com/a/33447587/3979819
		 */
		template <typename T, std::size_t N = sizeof(T) << 1>
		std::string to_hex(T w)
		{
			static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "arithmetic data type required");

			constexpr static char digits[] = "0123456789abcdef";
			std::string str(N, '0');
			for (size_t idx = 0, pos = (N - 1) * 4; idx < N; ++idx, pos -= 4) {
				str[idx] = digits[(w >> pos) & 0x0f];
			}
			return str;
		}

		/**
		 * Writes a binary buffer as hex values separated
		 * by a single character (preferrable a white space)
		 */
		std::string to_hex(buffer_t const&, char = ' ');

	}
}
#endif
