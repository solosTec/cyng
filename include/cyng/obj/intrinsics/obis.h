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

		constexpr obis(data_type d)
			: value_({ d.at(VG_MEDIUM), d.at(VG_CHANNEL), d.at(VG_INDICATOR), d.at(VG_MODE), d.at(VG_QUANTITY), d.at(VG_STORAGE) })
		{}

		obis(obis const&) = default;

		/**
		 *	Fill the first 6 bytes from a u64 value with the OBIS groups
		 */
		std::uint64_t to_uint64() const;

		/**
		 *	extract storage and quantity field into one integer
		 */
		std::uint16_t to_uint16() const;

		data_type const& data() const;

		constexpr value_type operator[](value_group vg) const {
			BOOST_ASSERT(vg < VG_EOG);
			return value_.at(vg);
		}

		constexpr value_type get_medium() const {
			return value_.at(VG_MEDIUM);
		}
		constexpr value_type get_channel() const {
			return value_.at(VG_CHANNEL);
		}
		constexpr value_type get_indicator() const {
			return value_.at(VG_INDICATOR);
		}
		constexpr value_type get_mode() const {
			return value_.at(VG_MODE);
		}
		constexpr value_type get_quantity() const {
			return value_.at(VG_QUANTITY);
		}
		constexpr value_type get_storage() const {
			return value_.at(VG_STORAGE);
		}

		/**
		 * @return true if the obis code starts with the specified buffer sequence
		 */
		bool starts_with(buffer_t) const;
		bool starts_with(std::initializer_list<std::uint8_t>) const;

	private:
		data_type	value_;

	};

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
	constexpr bool is_abstract(obis const& o) {
		return o.get_medium() == 0;
	}

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
	std::string to_string(obis const&);

	/**
	 * An OBIS code like "81 81 11 06 FF FF" has a depth of 2
	 * because there are two (2) "FF" values at the end.
	 * This actually allows to list 2^16 = 65536 elements.
	 */
	std::size_t depth(obis const&);

	/**
	 * If an OBIS code has a depth > 0 then descend() produces
	 * the code of the first entry in the list. If depth == 0
	 * the function returns the OBIS code unchanged.
	 * 
	 * Examples:
	 * "81 81 11 06 FF FF" => "81 81 11 06 01 FF"
	 * "81 81 11 06 01 FF" => "81 81 11 06 01 01"
	 */
	obis descend(obis const&);

	/**
	 * Produces the next element in a list.
	 * Examples:
	 * "81 81 11 06 01 FF" => "81 81 11 06 02 FF"
	 * "81 81 11 06 01 02" => "81 81 11 06 01 02"
	 */
	obis next(obis const&);

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

	constexpr obis make_obis(std::uint8_t a
		, std::uint8_t b
		, std::uint8_t c
		, std::uint8_t d
		, std::uint16_t ef) {

#if defined(cyng_BIG_ENDIAN)
		return obis(a, b, c, d, ef & 0xFF, (ef >> 8) & 0xFF);
#else
		return obis(a, b, c, d, (ef >> 8) & 0xFF, ef & 0xFF);
#endif
	}

	/**
	 * Generate an OBIS code from an existing code but change the storage field
	 *
	 * @param s storage value
	 */
	constexpr obis make_obis(obis const& code, std::uint8_t s) {
		return obis(code[obis::VG_MEDIUM]
			, code[obis::VG_CHANNEL]
			, code[obis::VG_INDICATOR]
			, code[obis::VG_MODE]
			, code[obis::VG_QUANTITY]
			, s);
	}


	/**
	 * Generate an OBIS code from an existing code but change the storage and quantity field
	 *
	 * @param s the value of s will be splitted in two 8 bit unsigned integers that will
	 * replace the last two values of the specified obis code.
	 */
	obis make_obis_2(obis const& code, std::uint16_t s);

	/**
	 * Reverse function of to_uint64()
	 */
	obis make_obis(std::uint64_t);

	/**
	 * definition of an OBIS path
	 */
	using obis_path_t = std::vector<obis>;

	/**
	 * Produce an obis path extended by the specified obis code.
	 */
	obis_path_t append(obis_path_t, obis);
	obis_path_t append(obis_path_t, obis_path_t);

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
