/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/version.h>

namespace cyng	{

}


namespace std {
	size_t hash<cyng::version>::operator()(cyng::version const& v) const noexcept
	{
		return std::hash<std::uint32_t>{}(v.full());
	}
	size_t hash<cyng::revision>::operator()(cyng::revision const& r) const noexcept
	{
		return std::hash<std::uint64_t>{}(r.full());
	}
}


