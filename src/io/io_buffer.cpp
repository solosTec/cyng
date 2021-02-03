/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/io/io_buffer.h>

namespace cyng {
	namespace io {

		std::string to_hex(buffer_t const& buffer, char sp)
		{
			//
			//	optimization to get the right string length
			//
			auto const size = (buffer.empty())
				? 1
				: ((buffer.size() * 2) + buffer.size() - 1)
				;

			std::string r;
			r.reserve(size);

			bool flag = false;
			for (const char c : buffer)
			{
				if (flag) {
					r.push_back(sp);
				}
				else {
					flag = true;
				}
				r += to_hex(c);
			}

			return r;
		}
	}
}

