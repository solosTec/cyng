/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_INTRINSCIS_AES_KEY_HPP
#define CYNG_OBJ_INTRINSCIS_AES_KEY_HPP	

#include <cstddef>
#include <array>
#include <type_traits>
#include <cyng/obj/intrinsics/buffer.h>

namespace cyng {

	/**
	 * @tparam N bitsize of AES key (possible values are 128, 196, 256)
	 */
	template <std::size_t N>
	struct aes_key
	{
		using value_type = std::uint8_t;
		using SIZE = std::integral_constant<std::size_t, N / 8>;
		using key_type = std::array<value_type, SIZE::value>;
		key_type	key_;

		/**
		 * Interface for ::AES_set_encrypt_key()
		 */
		constexpr static std::size_t size() noexcept
		{
			return N;
		}

		/**
		 * Default constructor
		 */
		constexpr aes_key()
			: key_({ { 0 } })
		{}

		constexpr aes_key(key_type const& key)
			: key_(key)
		{}

		constexpr aes_key(key_type&& key)
			: key_(std::forward<key_type>(key))
		{}

		/**
		 * Copy constructor
		 */
		constexpr aes_key(aes_key const& key)
			: key_(key.key_)
		{}

		/**
		 * Move constructor
		 */
		constexpr aes_key(aes_key&& key) noexcept
			: key_(std::move(key.key_))
		{}

		constexpr aes_key& operator=(aes_key const& key)
		{
			key_ = key.key_;
			return *this;
		}

	};

	template <std::size_t N>
	buffer_t to_buffer(aes_key<N> const& key)
	{
		return buffer_t(key.key_.begin(), key.key_.end());
	}

	namespace crypto
	{
		using aes_128_key = aes_key<128>;	//	16 bytes
		using aes_192_key = aes_key<192>;	//	24 bytes
		using aes_256_key = aes_key<256>;	//	32 bytes
	}

}

#include <functional>

namespace std {

	//
	//	partial specialization
	//
	template <std::size_t N>
	class hash<cyng::aes_key<N>> {
	public:
		size_t operator()(cyng::aes_key<N> const& key) const
		{
			using value_type = typename cyng::aes_key<N>::value_type;

			std::size_t h{ 0 };
			auto f = std::hash<value_type>{};
			for (auto const& c : key.key_) {

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
