/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_EDIS_H
#define CYNG_OBJ_INTRINSCIS_EDIS_H	

#include <cyng/obj/intrinsics/buffer.h>

#include <cstddef>
#include <type_traits>
#include <algorithm>
#include <boost/assert.hpp>

namespace cyng {

	class obis;
	class edis
	{
	public:
		using value_type = std::uint8_t;
		using SIZE = std::integral_constant<std::size_t, 3>;
		//	internal data type
		using data_type = std::array< std::uint8_t, SIZE::value >;

		enum value_group {
			//VG_MEDIUM = 0,		//	A
			//VG_CHANNEL = 1,		//	B
			VG_INDICATOR = 0,		//	C - metric (physcial value)
			VG_MODE = 1,			//	D - measurement mode
			VG_QUANTITY = 2,		//	E - tariff
			//VG_STORAGE = 5,		//	F
			VG_EOG				//	out of range
		};

	public:

		/**
		 * Interface for ::AES_set_encrypt_key()
		 */
		constexpr static std::size_t size() noexcept
		{
			return SIZE::value;
		}

		constexpr edis()
			: value_{ 0 }
		{}
		constexpr edis(std::uint8_t c
			, std::uint8_t d
			, std::uint8_t e)
			: value_{ c, d, e }
		{}

		edis(edis const&) = default;

		/**
		 *	Fill the first 6 bytes from a u64 value with the OBIS groups
		 */
		[[nodiscard]]
		std::uint64_t to_uint32() const;

		data_type const& data() const;

		value_type operator[](value_group) const;

	private:
		data_type	value_;

	};

	/**
	 * @return true if all elements are zero.
	 */
	bool is_nil(edis const&);

	/**
	 *	Create a buffer containing all 6 bytes of
	 *	the OBIS value.
	 */
	[[nodiscard]]
	buffer_t to_buffer(edis const&);

	//	comparison (constexpr since C++20)
	bool operator==(edis const& lhs, edis const& rhs) noexcept;
	bool operator<(edis const&, edis const&) noexcept;
	bool operator!=(edis const&, edis const&) noexcept;
	bool operator>(edis const&, edis const&) noexcept;
	bool operator<=(edis const&, edis const&) noexcept;
	bool operator>=(edis const&, edis const&) noexcept;

	/**
	 * extract value group C, D and E
	 */
	[[nodiscard]]
	edis to_edis(obis const&);

}

#include <functional>

namespace std {

	template <> 
	class hash<cyng::edis> {
	public:
		size_t operator()(cyng::edis const&) const noexcept;
	};
}

#endif 
