/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_INTRINSCIS_DIGEST_HPP
#define DOCC_OBJ_INTRINSCIS_DIGEST_HPP	

#include <cstddef>
#include <array>

namespace docscript {

	template <std::size_t N>
	struct digest
	{
		using value_type = std::uint8_t;
		using digest_type = std::array<value_type, N>;

		constexpr static std::size_t size() noexcept
		{
			return N;
		}

		constexpr digest()
			: digest_{ 0 }
		{}

		constexpr digest(value_type const& v)
			: digest_{ v }
		{}

		constexpr digest(digest_type const& d)
			: digest_( d )
		{}

		constexpr digest(value_type&& v)
			: digest_( std::move(v) )
		{}

		constexpr digest(digest const& d)
			: digest_{ d.digest_ }
		{}

		digest_type digest_;
	};

	namespace crypto
	{
		using digest_md5 = digest<16>;
		using digest_sha1 = digest<20>;
		using digest_sha256 = digest<32>;
		using digest_sha512 = digest<64>;
	}

}

#include <functional>

namespace std {

	//
	//	partial specialization
	//
	template <std::size_t N>
	class hash<docscript::digest<N>> {
	public:
		size_t operator()(docscript::digest<N> const& d) const
		{
			using value_type = typename docscript::digest<N>::value_type;

			std::size_t h{ 0 };
			auto f = std::hash<value_type>{};
			for (auto const& c : d.digest_) {

				//
				//	combine all values
				//
				h ^= f(c) << 1;
			}
			return h;
		}
	};
}

#endif 
