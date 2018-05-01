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
#include <cyng/intrinsics/buffer.h>
#include <array>
#include <cstdint>

namespace cyng
{
	namespace crypto
	{
		struct aes_128_key
		{
			//	16 bytes
			using key_type = std::array<std::uint8_t, 128 / 8>;
			key_type	aes_key_;
			
			aes_128_key()
			: aes_key_({ { 0 } })
			{}
			
			aes_128_key(key_type const& key)
			: aes_key_(key)
			{}
			
			aes_128_key(key_type&& key)
			: aes_key_(std::forward<key_type>(key))
			{}
		};

		struct aes_192_key
		{
			//	24 bytes
			using key_type = std::array<std::uint8_t, 192 / 8>;
			key_type aes_key_;
			
			aes_192_key()
			: aes_key_({ { 0 } })
			{}
			
			aes_192_key(key_type const& key)
			: aes_key_(key)
			{}
			
			aes_192_key(key_type&& key)
			: aes_key_(std::forward<key_type>(key))
			{}
		};

		struct aes_256_key
		{
			//	32 bytes
			using key_type = std::array<std::uint8_t, 256 / 8>;
			key_type aes_key_;

			aes_256_key()
			: aes_key_({ { 0 } })
			{}
			
			aes_256_key(key_type const& key)
			: aes_key_(key)
			{}
			
			aes_256_key(key_type&& key)
			: aes_key_(std::forward<key_type>(key))
			{}
		};
	}
}

#endif	

