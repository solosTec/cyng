/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/obis.h>
#include <cyng/obj/intrinsics/edis.h>
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/io/io_buffer.h>

#include <numeric>

namespace cyng	{

	std::uint64_t obis::to_uint64() const
	{
		//
		//	use only the lower 6 bytes
		//
		std::uint64_t r{ 0 };

		//	check network byte ordering
		//
#if defined(docc_BIG_ENDIAN)
		std::memcpy(&r, value_.data(), obis::size());
#else
		data_type tmp{ 0 };
		std::reverse_copy(std::begin(value_), std::end(value_), tmp.begin());
		std::memcpy(&r, tmp.data(), obis::size());
#endif
		BOOST_ASSERT(r < 0xFFFFFFFFFFFF);
		return r;
	}

	obis::data_type const& obis::data() const {
		return value_;
	}

	bool is_private(obis const& o) {
		return 	(o[obis::VG_MEDIUM] >= 0x80 && o[obis::VG_MEDIUM] <= 0xC7)
			|| (o[obis::VG_CHANNEL] >= 0x80 && o[obis::VG_CHANNEL] <= 0xC7)
			|| (o[obis::VG_INDICATOR] == 0xF0)
			|| (o[obis::VG_MODE] >= 0x80 && o[obis::VG_MODE] <= 0xFE)
			|| (o[obis::VG_QUANTITY] >= 0x80 && o[obis::VG_QUANTITY] <= 0xFE)
			|| (o[obis::VG_STORAGE] >= 0x80 && o[obis::VG_STORAGE] <= 0xFE)
			;
	}

	bool is_nil(obis const& o) {
		return std::all_of(std::begin(o.data()), std::end(o.data()), [](std::uint8_t c) {
			return c == 0u;
			});
	}

	bool is_abstract(obis const& o) {
		return o[obis::VG_MEDIUM] == 0;
	}

	buffer_t to_buffer(obis const& o) {
		return to_buffer(o.data());
	}

	std::string to_str(obis const& o) {
		return std::accumulate(std::begin(o.data()), std::end(o.data()), std::string(), [](std::string r, std::uint8_t c) {
			return r + io::to_hex(c);
			});
	}

	//	comparison
	bool operator==(obis const& lhs, obis const& rhs) noexcept
	{
		return lhs.data() == rhs.data();
	}
	bool operator<(obis const& lhs, obis const& rhs) noexcept
	{
		return lhs.data() < rhs.data();
	}
	bool operator!=(obis const& lhs, obis const& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	bool operator>(obis const& lhs, obis const& rhs) noexcept
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(obis const& lhs, obis const& rhs) noexcept
	{
		return !(lhs > rhs);
	}
	bool operator>=(obis const& lhs, obis const& rhs) noexcept
	{
		return !(lhs < rhs);
	}


}


namespace std {
	size_t hash<cyng::obis>::operator()(cyng::obis const& v) const noexcept
	{
		return v.to_uint64();
	}
	//bool equal_to<cyng::obis>::operator()(cyng::obis const& c1, cyng::obis const& c2) const noexcept
	//{
	//	return c1.equal(c2);
	//}
	//bool less<cyng::obis>::operator()(cyng::obis const& c1, cyng::obis const& c2) const noexcept
	//{
	//	return c1.less(c2);
	//}

}


