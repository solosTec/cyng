/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_DIGEST_HPP
#define CYNG_OBJ_INTRINSCIS_DIGEST_HPP	

#include <cstddef>
#include <array>

namespace cyng {

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
		constexpr std::size_t md5_size = 16;
		constexpr std::size_t sha1_size = 20;
		constexpr std::size_t sha256_size = 32;
		constexpr std::size_t sha512_size = 64;

		using digest_md5 = digest<md5_size>;
		using digest_sha1 = digest<sha1_size>;
		using digest_sha256 = digest<sha256_size>;
		using digest_sha512 = digest<sha512_size>;
	}

}

#include <functional>

namespace std {

	//
	//	partial specialization
	//
	template <std::size_t N>
	class hash<cyng::digest<N>> {
	public:
		size_t operator()(cyng::digest<N> const& d) const
		{
			using value_type = typename cyng::digest<N>::value_type;

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
