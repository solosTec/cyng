/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/pid.h>

namespace cyng	{

	//	comparison
	bool operator==(pid const& lhs, pid const& rhs) noexcept
	{
		return lhs.operator std::uint64_t() == rhs.operator std::uint64_t();
	}
	bool operator<(pid const& lhs, pid const& rhs) noexcept
	{
		return lhs.operator std::uint64_t() < rhs.operator std::uint64_t();
	}
	bool operator!=(pid const& lhs, pid const& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	bool operator>(pid const& lhs, pid const& rhs) noexcept
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(pid const& lhs, pid const& rhs) noexcept
	{
		return !(lhs > rhs);
	}
	bool operator>=(pid const& lhs, pid const& rhs) noexcept
	{
		return !(lhs < rhs);
	}

}


namespace std {
	size_t hash<cyng::pid>::operator()(cyng::pid const& v) const noexcept
	{
		return static_cast<size_t>(v.operator std::uint64_t());
	}
}


