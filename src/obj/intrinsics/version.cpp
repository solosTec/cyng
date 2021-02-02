/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/version.h>

namespace docscript	{

}


namespace std {
	size_t hash<docscript::version>::operator()(docscript::version const& v) const noexcept
	{
		return std::hash<std::uint32_t>{}(v.full());
	}
	size_t hash<docscript::revision>::operator()(docscript::revision const& r) const noexcept
	{
		return std::hash<std::uint64_t>{}(r.full());
	}
}


