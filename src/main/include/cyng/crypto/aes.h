/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

/** @file aes.h
 *  AES - Advanced Encryption Standard
 */

#ifndef CYNG_CRYPTO_AES_H
#define CYNG_CRYPTO_AES_H


#include <cyng/crypto/aes_keys.h>

namespace cyng
{
	namespace crypto
	{
		void encrypt(buffer_t&, buffer_t const&, aes_256_key const&);
		void decrypt(buffer_t&, buffer_t const&, aes_256_key const&);
	}
}

#endif	

