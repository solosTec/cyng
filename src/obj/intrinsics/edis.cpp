/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <cyng/obj/intrinsics/edis.h>
#include <cyng/obj/buffer_cast.hpp>
#include <cyng/io/io_buffer.h>
#include <cyng/obj/intrinsics/obis.h>

#include <numeric>

namespace cyng	{

	std::uint64_t edis::to_uint32() const
	{
		//
		//	use only the lower 6 bytes
		//
		std::uint32_t r{ 0 };

		//	check network byte ordering
		//
#if defined(cyng_BIG_ENDIAN)
		std::memcpy(&r, value_.data(), obis::size());
#else
		data_type tmp{ 0 };
		std::reverse_copy(std::begin(value_), std::end(value_), tmp.begin());
		std::memcpy(&r, tmp.data(), edis::size());
#endif
		BOOST_ASSERT(r < 0xFFFFFFFFFFFF);
		return r;
	}

	edis::data_type const& edis::data() const {
		return value_;
	}

	edis::value_type edis::operator[](edis::value_group vg) const {
		BOOST_ASSERT(vg < VG_EOG);
		return value_[vg];
	}

	bool is_nil(edis const& o) {
		return std::all_of(std::begin(o.data()), std::end(o.data()), [](std::uint8_t c) {
			return c == 0u;
			});
	}

	buffer_t to_buffer(edis const& o) {
		return to_buffer(o.data());
	}

	//	comparison
	bool operator==(edis const& lhs, edis const& rhs) noexcept
	{
		return lhs.data() == rhs.data();
	}
	bool operator<(edis const& lhs, edis const& rhs) noexcept
	{
		return lhs.data() < rhs.data();
	}
	bool operator!=(edis const& lhs, edis const& rhs) noexcept
	{
		return !(lhs == rhs);
	}
	bool operator>(edis const& lhs, edis const& rhs) noexcept
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(edis const& lhs, edis const& rhs) noexcept
	{
		return !(lhs > rhs);
	}
	bool operator>=(edis const& lhs, edis const& rhs) noexcept
	{
		return !(lhs < rhs);
	}

	edis to_edis(obis const& o) {
		return edis(o[obis::VG_INDICATOR], o[obis::VG_MODE], o[obis::VG_QUANTITY]);
	}

}


namespace std {
	size_t hash<cyng::edis>::operator()(cyng::edis const& v) const noexcept
	{
		return v.to_uint32();
	}
}


