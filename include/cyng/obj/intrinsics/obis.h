/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_OBIS_H
#define CYNG_OBJ_INTRINSCIS_OBIS_H	

#include <cyng/obj/intrinsics/buffer.h>

#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <vector>

#include <boost/assert.hpp>

namespace cyng {

	class obis
	{
	public:
		using value_type = std::uint8_t;
		using SIZE = std::integral_constant<std::size_t, 6>;
		//	internal data type
		using data_type = std::array< value_type, SIZE::value >;

		enum value_group {
			VG_MEDIUM = 0,		//	A - 0 .. 15
			VG_CHANNEL = 1,		//	B - channel 0 .. 255
			VG_INDICATOR = 2,	//	C - metric (physical value)
			VG_MODE = 3,		//	D - measurement mode
			VG_QUANTITY = 4,	//	E - tariff
			VG_STORAGE = 5,		//	F
			VG_EOG				//	out of range
		};

		/**
		 * Value group A codes
		 */
		enum media : std::uint8_t {
			MEDIA_ABSTRACT = 0,
			MEDIA_ELECTRICITY = 1,
			MEDIA_HEAT_COST_ALLOCATOR = 4,
			MEDIA_COOLING = 5,
			MEDIA_HEAT = 6,
			MEDIA_GAS = 7,
			MEDIA_WATER_COLD = 8,
			MEDIA_WATER_HOT = 9
		};

	public:

		/**
		 * Interface for ::AES_set_encrypt_key()
		 */
		constexpr static std::size_t size() noexcept
		{
			return SIZE::value;
		}

		constexpr obis()
			: value_{ 0 }
		{}
		constexpr obis(std::uint8_t a
			, std::uint8_t b
			, std::uint8_t c
			, std::uint8_t d
			, std::uint8_t e
			, std::uint8_t f)
			: value_{ a, b, c, d, e, f }
		{
			//BOOST_ASSERT(a < 16);
		}

		obis(obis const&) = default;

		/**
		 *	Fill the first 6 bytes from a u64 value with the OBIS groups
		 */
		[[nodiscard]]
		std::uint64_t to_uint64() const;

		data_type const& data() const;

		constexpr value_type operator[](value_group vg) const {
			BOOST_ASSERT(vg < VG_EOG);
			return value_[vg];
		}

		/**
		 * @return true if the obis code starts with the specified buffer sequence
		 */
		bool starts_with(buffer_t) const;

	private:
		data_type	value_;

	};

	/**
	 * definition of an OBIS path
	 */
	using obis_path_t = std::vector<obis>;

	/**
	 *	@return true if OBIS code has no global definition.
	 */
	bool is_private(obis const&);

	/**
	 * @return true if all elements are zero.
	 */
	bool is_nil(obis const&);

	/**
	 * @return true if medium is null
	 */
	bool is_abstract(obis const&);

	/**
	 *	Create a buffer containing all 6 bytes of
	 *	the OBIS value.
	 */
	[[nodiscard]]
	buffer_t to_buffer(obis const&);

	/**
	 * Create a hex string without spaces.
	 *
	 * @see io_buffer.h
	 */
	[[nodiscard]]
	std::string to_str(obis const&);

	//	comparison (constexpr since C++20)
	bool operator==(obis const& lhs, obis const& rhs) noexcept;
	bool operator<(obis const&, obis const&) noexcept;
	bool operator!=(obis const&, obis const&) noexcept;
	bool operator>(obis const&, obis const&) noexcept;
	bool operator<=(obis const&, obis const&) noexcept;
	bool operator>=(obis const&, obis const&) noexcept;


	/**
	 * compare first n elements of the obis codes
	 */
	bool compare_n(obis const& lhs, obis const& rhs, std::size_t n);

	/**
	 * Generate an OBIS code
	 */
	constexpr obis make_obis(std::uint32_t a
		, std::uint32_t b
		, std::uint32_t c
		, std::uint32_t d
		, std::uint32_t e
		, std::uint32_t f) {

		return obis(static_cast<std::uint8_t>(a & 0xFF)
			, static_cast<std::uint8_t>(b & 0xFF)
			, static_cast<std::uint8_t>(c & 0xFF)
			, static_cast<std::uint8_t>(d & 0xFF)
			, static_cast<std::uint8_t>(e & 0xFF)
			, static_cast<std::uint8_t>(f & 0xFF));
	}

	/**
	 * Generate an OBIS code from an existing code but change the storage field
	 *
	 * @param s storage value
	 */
	constexpr obis make_obis(obis const& code, std::uint32_t s) {
		return obis(code[obis::VG_MEDIUM]
			, code[obis::VG_CHANNEL]
			, code[obis::VG_INDICATOR]
			, code[obis::VG_MODE]
			, code[obis::VG_QUANTITY]
			, static_cast<std::uint8_t>(s & 0xFF));
	}

}

#include <functional>

namespace std {

	template <> 
	class hash<cyng::obis> {
	public:
		size_t operator()(cyng::obis const&) const noexcept;
	};
	template <>
	class hash<cyng::obis_path_t> {
	public:
		size_t operator()(cyng::obis_path_t const&) const noexcept;
	};
}

#endif 
