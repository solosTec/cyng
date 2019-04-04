/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef CYNG_CRYPTO_AES_KEYS_H
#define CYNG_CRYPTO_AES_KEYS_H


#include <string>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

#include <cyng/intrinsics/buffer.h>
#include <array>
#include <cstdint>

namespace cyng
{
	namespace crypto
	{
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

			/**
			 * Default constructor
			 */
			aes_key()
				: key_({ { 0 } })
			{}

			aes_key(key_type const& key)
				: key_(key)
			{}

			aes_key(key_type&& key)
				: key_(std::forward<key_type>(key))
			{}

			/**
			 * Copy constructor
			 */
			aes_key(aes_key const& key)
				: key_(key.key_)
			{}

			/**
			 * Fill with a random value
			 */
			void randomize()
			{
				::RAND_bytes(key_.data(), N);
			}

		};

		using aes_128_key = aes_key<128>;	//	16 bytes
		using aes_192_key = aes_key<192>;	//	24 bytes
		using aes_256_key = aes_key<256>;	//	32 bytes

	}
}

#endif	

