/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/io/serializer/json.hpp>
#include <cyng/io/serializer/plain.hpp>

#include <boost/io/ios_state.hpp>

namespace cyng {
	namespace io {

		std::size_t serializer <bool, JSON>::write(std::ostream& os, bool v)
		{
			return serializer<bool, PLAIN>::write(os, v);
		}
	}
}
