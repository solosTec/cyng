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
#include <cyng/intrinsics/buffer.h>

#include <openssl/aes.h>

namespace cyng
{
	namespace crypto
	{
		namespace aes
		{
			/**
			 * initial vector for AES CBC operations
			 */
			using iv_t = std::array<std::uint8_t, AES_BLOCK_SIZE>;

			constexpr std::size_t adjust_buffer_size(std::size_t length) {
				return ((length + AES_BLOCK_SIZE) / AES_BLOCK_SIZE) * AES_BLOCK_SIZE;
			}

			/**
			 * Fill key with random values
			 */
			void randomize(aes_128_key&);
			void randomize(aes_192_key&);
			void randomize(aes_256_key&);
			void randomize(iv_t&);

			template <typename KEY>
			KEY make_aes_key(cyng::buffer_t const& buffer) {
				KEY key;
				if (buffer.size() == key.key_.size()) {
					std::copy(buffer.begin(), buffer.end(), key.key_.begin());
				}
				return key;
			}


			/**
			 * @brief encrypt AES ECB
			 * Only blocks with 16 bytes supported.
			 * @see https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption
			 * @see https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption
			 */
			template <typename KEY>
			cyng::buffer_t encrypt(buffer_t const& inp, KEY const& key)
			{
				//
				//	adjust buffer size for encrypted output
				//
				size_t const length = adjust_buffer_size(inp.size());
				cyng::buffer_t out;

				AES_KEY enc_key;
				if (0 == AES_set_encrypt_key(key.get_key(), key.size(), &enc_key)) {
					out.resize(length);
					AES_encrypt((const unsigned char*)inp.data(), (unsigned char*)out.data(), &enc_key);
				}

				return out;
			}

			/**
			 * @brief decrypt AES ECB
			 * Only blocks with 16 bytes supported.
			 */
			template <typename KEY>
			cyng::buffer_t decrypt(buffer_t const& inp, KEY const& key)
			{
				//
				//	adjust buffer size
				//
				cyng::buffer_t out;

				AES_KEY dec_key;
				if (0 == AES_set_decrypt_key(key.get_key(), key.size(), &dec_key)) {
					out.resize(inp.size());	//	probably less
					AES_decrypt((const unsigned char*)inp.data(), (unsigned char*)out.data(), &dec_key);
				}
				return out;
			}

			/**
			 * AES CBC encryption
			 */
			template <typename KEY>
			cyng::buffer_t encrypt(buffer_t const& inp, KEY const& key, iv_t const& iv)
			{
				//
				//	adjust buffer size for encrypted output
				//
				size_t const length = adjust_buffer_size(inp.size());
				cyng::buffer_t out;

				//
				// set encryption key
				//
				AES_KEY enc_key;
				if (0 == AES_set_encrypt_key(key.get_key(), key.size(), &enc_key)) {


					//
					//	the initialization vector is modified after an encrypt / decrypt operation
					//
					iv_t iv_cpy = iv;
					out.resize(length);

					AES_cbc_encrypt((const unsigned char*)inp.data()
						, (unsigned char*)out.data()
						, inp.size()
						, &enc_key
						, iv_cpy.data()
						, AES_ENCRYPT);
				}

				return out;

			}

			/**
			 * AES CBC decryption
			 */
			template <typename KEY>
			cyng::buffer_t decrypt(buffer_t const& inp, KEY const& key, iv_t const& iv)
			{
				//
				//	adjust buffer size
				//
				cyng::buffer_t out;

				AES_KEY dec_key;
				if (0 == AES_set_decrypt_key(key.get_key(), key.size(), &dec_key)) {

					//
					//	the initialization vector is modified after an encrypt / decrypt operation
					//
					iv_t iv_cpy = iv;
					out.resize(inp.size());	//	probably less

					AES_cbc_encrypt((const unsigned char*)inp.data()
						, (unsigned char*)out.data()
						, inp.size()
						, &dec_key
						, iv_cpy.data()
						, AES_DECRYPT);
				}

				return out;
			}

		}
	}
}

#endif	

