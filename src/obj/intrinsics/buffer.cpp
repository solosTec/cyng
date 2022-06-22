/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/buffer.h>
#include <algorithm>

namespace cyng	{
	buffer_t make_buffer(std::initializer_list<std::uint8_t> ilist)
	{
		buffer_t tmp;
		tmp.reserve(ilist.size());
		std::for_each(ilist.begin(), ilist.end(), [&](std::uint8_t c) {
			tmp.push_back(reinterpret_cast<char &>(c));
		});
		return tmp;
	}

	buffer_t make_buffer(std::string const& s)
	{
		return buffer_t(s.begin(), s.end());
	}


	bool is_ascii(buffer_t const& v)
	{
		return std::all_of(v.cbegin(), v.cend(), [](char c) { 
			return (c > 31) && (c < 126);
		});
	}

	bool is_null(buffer_t const& v) {
		return std::all_of(v.cbegin(), v.cend(), [](char c) { return c == 0; });
	}

	std::size_t hash(buffer_t const& buffer)
	{
		std::size_t h{ 0 };
		auto f = std::hash<buffer_t::value_type>{};
		for (auto const& c : buffer) {

			//
			//	combine all values
			//
			h ^= f(c) << 1;
		}
		return h;
	}

	buffer_t clone(buffer_t const& buffer) {
		buffer_t r;
		r.reserve(buffer.size());
		r.assign(buffer.begin(), buffer.end());
		return r;
	}

}


namespace std {

	size_t hash<cyng::buffer_t>::operator()(cyng::buffer_t const& v) const
	{
			return cyng::hash(v);
	}
}


