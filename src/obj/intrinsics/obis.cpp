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
#include <cyng/obj/array_cast.hpp>

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
#if defined(cyng_BIG_ENDIAN)
		std::memcpy(&r, value_.data(), obis::size());
#else
		data_type tmp{ 0 };
		std::reverse_copy(std::begin(value_), std::end(value_), tmp.begin());
		std::memcpy(&r, tmp.data(), obis::size());
#endif
		BOOST_ASSERT(r < 0xFFFFFFFFFFFF);
		return r;
	}

	std::uint16_t obis::to_uint16() const {
		//
		//	use only storage and quantity field
		//

		//	check network byte ordering
		//
#if defined(cyng_BIG_ENDIAN)
		return (value_.at(VG_STORAGE) << 8) | value_.at(VG_QUANTITY);
#else
		return (value_.at(VG_QUANTITY) << 8) | value_.at(VG_STORAGE);
#endif
	}


	obis::data_type const& obis::data() const {
		return value_;
	}

	bool obis::starts_with(buffer_t seq) const {
		if (seq.size() < size()) {
			return std::equal(seq.begin(), seq.end(), value_.begin(), [](char c1, std::uint8_t c2) {
				return static_cast<std::uint8_t>(c1) == c2;
				});
		}
		return false;
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

	bool compare_n(obis const& lhs, obis const& rhs, std::size_t n) {
		if (n < obis::size()) {
			return std::equal(lhs.data().begin(), lhs.data().begin() + n, rhs.data().begin());
		}
		return false;
	}

	std::size_t depth(obis const& o) {
		std::size_t d = 0;
		for (auto pos = o.data().rbegin(); pos != o.data().rend(); ++pos) {
			if (*pos == 0xff) {
				++d;
			}
			else {
				break;
			}
		}
		return d;
	}

	obis descend(obis const& o) {
		//	copy
		obis::data_type d = o.data();
		for (auto pos = d.rbegin(); pos != d.rend(); ++pos) {
			if (*pos != 0xff && pos != d.rbegin()) {
				*--pos = 0x01;	//	index starts with 1
				return d;	//	implicit conversion to obis type
			}
		}
		return o;
	}

	/**
	 * Produces the next element in a list.
	 * Examples:
	 * "81 81 11 06 01 FF" => "81 81 11 06 02 FF"
	 * "81 81 11 06 01 02" => "81 81 11 06 01 02"
	 */
	obis next(obis const& o) {
		//	copy
		obis::data_type d = o.data();
		for (auto pos = d.rbegin(); pos != d.rend(); ++pos) {
			if (*pos != 0xff && pos != d.rbegin()) {
				++(* pos);
				return d;	//	implicit conversion to obis type
			}
		}
		return o;
	}

	obis make_obis_2(obis const& code, std::uint16_t s) {
		auto const a = to_array<std::uint8_t>(s);
		return obis(code[obis::VG_MEDIUM]
			, code[obis::VG_CHANNEL]
			, code[obis::VG_INDICATOR]
			, code[obis::VG_MODE]
			, a.at(0)
			, a.at(1));
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

	obis_path_t append(obis_path_t p, obis code) {
		p.push_back(code);
		return p;
	}

	obis_path_t append(obis_path_t p, obis_path_t e) {
		p.insert(p.end(), e.begin(), e.end());
		return p;
	}
}


namespace std {
	size_t hash<cyng::obis>::operator()(cyng::obis const& v) const noexcept
	{
		return v.to_uint64();
	}
	size_t hash<cyng::obis_path_t>::operator()(cyng::obis_path_t const& v) const noexcept
	{
		std::size_t h{ 0 };
		for (auto const& o : v) {

			//
			//	combine all values
			//
			h ^= o.to_uint64() << 1;
		}
		return h;
	}
}


