/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/program.h>


namespace std {
	size_t hash<cyng::prg_t>::operator()(cyng::prg_t const& prg) const
	{
		auto const f = std::hash<cyng::buffer_t>{};
		std::size_t h{ 0 };
		for (auto const& c : prg) {

			//
			//	combine all values
			//
			h ^= f(c) << 1;
		}
		return h;
	}

}


