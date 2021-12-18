/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/raw.h>
#include <cyng/obj/tag.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/functional/hash.hpp>

namespace cyng	{

	raw::raw(std::string lit, std::uint16_t code) noexcept
		: literal_(lit)
		, code_(code)
	{}

	raw::raw(std::string lit, std::string code)
		: literal_(lit)
		, code_(type_code_by_name(code))
	{}


	std::string const& raw::get_literal() const noexcept {
		return literal_;
	}

	std::string raw::get_code_name() const {
		return intrinsic_name_by_type_code(static_cast<type_code>(code_));
	}

	//	comparison
	bool operator==(raw const& lhs, raw const& rhs) noexcept
	{
		return (lhs.get_code() == rhs.get_code()
			? boost::algorithm::equals(lhs.get_literal(), rhs.get_literal())
			: false);
	}
	bool operator<(raw const& lhs, raw const& rhs) noexcept
	{
		return (lhs.get_code() < rhs.get_code()
			? boost::algorithm::lexicographical_compare(lhs.get_literal(), rhs.get_literal())
			: false);
	}
	bool operator!=(raw const& lhs, raw const& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	bool operator>(raw const& lhs, raw const& rhs) noexcept
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(raw const& lhs, raw const& rhs) noexcept
	{
		return !(lhs > rhs);
	}
	bool operator>=(raw const& lhs, raw const& rhs) noexcept
	{
		return !(lhs < rhs);
	}

}


namespace std {
	size_t hash<cyng::raw>::operator()(cyng::raw const& v) const noexcept
	{
		std::size_t seed = 0;
		boost::hash_combine(seed, v.get_code());
		boost::hash_combine(seed, v.get_literal());
		return seed;
	}
}


