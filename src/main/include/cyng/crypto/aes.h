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
        /**
         * @brief encrypt
         * Only blocks with 16 bytes supported.
         * @see https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption
         * @see https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
         */
		void encrypt(buffer_t&, buffer_t const&, aes_256_key const&);

        /**
         * @brief decrypt
         * Only blocks with 16 bytes supported.
         */
		void decrypt(buffer_t&, buffer_t const&, aes_256_key const&);
	}
}

#endif	

