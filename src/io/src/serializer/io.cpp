/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/io/io.h>

namespace cyng {
	namespace io {


		calc_size::calc_size(std::ostream& os)
			: pos_(os.tellp())
			, os_(os)
		{}

		calc_size::operator std::size_t() const {
			return count();
		}

		std::size_t calc_size::count() const {
			return os_.tellp() - pos_;
		}

	}
}
