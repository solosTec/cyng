/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#include <cyng/parse/buffer.h>
#include <cyng/parse/hex.h>

#include <algorithm>

#include <boost/assert.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng {

	buffer_t hex_to_buffer(std::string const& str) {

		auto const size = str.size();
		BOOST_ASSERT(size % 2 == 0);
		buffer_t r;
		if ((size % 2) != 0)	return r;

		if (size > 0)	r.reserve(size / 2);

		for (auto pos = str.begin(); pos != str.end(); pos += 2) {
			r.push_back(hex_to_u8(*pos, *(pos + 1)));
		}

		return r;
	}

}

