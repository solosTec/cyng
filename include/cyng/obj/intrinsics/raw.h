/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_RAW_H
#define CYNG_OBJ_INTRINSCIS_RAW_H	

#include <cstdint>
#include <cstddef>
#include <string>

/**
 * Be carefull to NOT include <object.h> or <intrinsics.h> since this is used as an intrinsic data type in tag.hpp.
 * Implement utility functions to create objects from this raw type, etc. outside this class.
 */

namespace cyng {

	/** @brief  unparsed data type
	 * 
	 * Contains a type information and an unparsed string
	 * to instantiate the value on demand.
	 */
	class raw {

	public:
		using value_type = std::uint64_t;

	public:
		/**
		 * cannot use the "type_code" enum here because "raw" is an intrinsic
		 * data type by itself.
		 */
		raw(std::string lit, std::uint16_t code) noexcept;
		raw(std::string lit, std::string code);
		raw(raw const&) = default;

		raw& operator=(raw const&) = default;

		/**
		 *  as defined in tag.hpp
		 */
		constexpr operator std::uint16_t() const noexcept {
			return get_code();
		}

		constexpr std::uint16_t get_code() const noexcept {
			return code_;
		}

		std::string const& get_literal() const noexcept;
		std::string get_code_name() const;

	private:
		std::string literal_;
		std::uint16_t code_;	//	type identifier as defined in tag.hpp
	};

	//	comparison
	bool operator==(raw const&, raw const&) noexcept;
	bool operator<(raw const&, raw const&) noexcept;
	bool operator!=(raw const&, raw const&) noexcept;
	bool operator>(raw const&, raw const&) noexcept;
	bool operator<=(raw const&, raw const&) noexcept;
	bool operator>=(raw const&, raw const&) noexcept;

}

#include <functional>

namespace std {

	/**
	 * make this class compatible with the cyng type system
	 */
	template <>
	class hash<cyng::raw> {
	public:
		size_t operator()(cyng::raw const&) const noexcept;
	};
}

#endif 
