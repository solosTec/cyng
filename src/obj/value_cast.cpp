/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#include <cyng/obj/value_cast.hpp>

namespace docscript {

	std::string value_cast(object const& obj, char const* p) noexcept
	{
		return value_cast<std::string>(obj, std::string(p));
	}
}

