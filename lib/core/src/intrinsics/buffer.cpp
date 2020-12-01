/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/buffer.h>
#include <algorithm>

namespace cyng 
{
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
}




