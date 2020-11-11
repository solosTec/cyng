/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

 /** @file aes_keys.h
  * Keep this file clean from references to OpenSLL or other
  *	libraries. Here are only type definitions. So this file can 
  * be included in <intrinsics.h>
  */

#ifndef CYNG_CRYPTO_AES_KEYS_H
#define CYNG_CRYPTO_AES_KEYS_H

#include <cyng/intrinsics/buffer.h>

#include <array>
#include <cstdint>

namespace cyng
{
	namespace crypto
	{
		/**
		 * @tparam N bitsize of AES key (possible values are 128, 196, 256)
		 */
		template <std::size_t N>
		struct aes_key
		{
			using key_type = std::array<std::uint8_t, N / 8>;
			key_type	key_;

			/**
			 * Interface for ::AES_set_encrypt_key()
			 */
			constexpr int size() const
			{
				return N;
			}

			/**
			 * Interface for ::AES_set_encrypt_key()
			 */
			constexpr unsigned char const* get_key() const
			{
				return key_.data();
			}

			buffer_t to_buffer() const
			{
				return buffer_t(key_.begin(), key_.end());
			}

			/**
			 * Default constructor
			 */
			aes_key() noexcept
				: key_({ { 0 } })
			{}

			explicit aes_key(key_type const& key)
				: key_(key)
			{}

			aes_key(key_type&& key) noexcept
				: key_(std::forward<key_type>(key))
			{}

			/**
			 * Copy constructor
			 */
			aes_key(aes_key const& key) noexcept
				: key_(key.key_)
			{}


		};

		using aes_128_key = aes_key<128>;	//	16 bytes
		using aes_192_key = aes_key<192>;	//	24 bytes
		using aes_256_key = aes_key<256>;	//	32 bytes

	}
}

#include <functional>

namespace std
{
	template<>
	struct equal_to<cyng::crypto::aes_128_key>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::aes_128_key;
		using second_argument_type = cyng::crypto::aes_128_key;

		inline bool operator()(cyng::crypto::aes_128_key const& c1, cyng::crypto::aes_128_key const& c2) const noexcept
		{
			return c1.key_ == c2.key_;
		}
	};

	template<>
	struct equal_to<cyng::crypto::aes_192_key>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::aes_192_key;
		using second_argument_type = cyng::crypto::aes_192_key;

		inline bool operator()(cyng::crypto::aes_192_key const& c1, cyng::crypto::aes_192_key const& c2) const noexcept
		{
			return c1.key_ == c2.key_;
		}
	};

	template<>
	struct equal_to<cyng::crypto::aes_256_key>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::aes_256_key;
		using second_argument_type = cyng::crypto::aes_256_key;

		inline bool operator()(cyng::crypto::aes_256_key const& c1, cyng::crypto::aes_256_key const& c2) const noexcept
		{
			return c1.key_ == c2.key_;
		}
	};

	template<>
	struct less<cyng::crypto::aes_128_key>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::aes_128_key;
		using second_argument_type = cyng::crypto::aes_128_key;

		inline bool operator()(cyng::crypto::aes_128_key const& c1, cyng::crypto::aes_128_key const& c2) const noexcept
		{
			return c1.key_ < c2.key_;
		}
	};

	template<>
	struct less<cyng::crypto::aes_192_key>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::aes_192_key;
		using second_argument_type = cyng::crypto::aes_192_key;

		inline bool operator()(cyng::crypto::aes_192_key const& c1, cyng::crypto::aes_192_key const& c2) const noexcept
		{
			return c1.key_ < c2.key_;
		}
	};

	template<>
	struct less<cyng::crypto::aes_256_key>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::aes_256_key;
		using second_argument_type = cyng::crypto::aes_256_key;

		inline bool operator()(cyng::crypto::aes_256_key const& c1, cyng::crypto::aes_256_key const& c2) const noexcept
		{
			return c1.key_ < c2.key_;
		}
	};

}

#endif	

