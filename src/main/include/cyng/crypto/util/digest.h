/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#ifndef CYNG_CRYPTO_DIGEST_H
#define CYNG_CRYPTO_DIGEST_H

#include <cyng/crypto.h>
#include <string>
#include <vector>

namespace cyng
{
	namespace crypto
	{
		/** @brief encrypted hash
		 *
		 * Create the SHA 256 digest and digital signature with an RSA object.
		 *
		 * @note the RSA object is no longer valid when function returns.
		 *
		 * @param digest_name digest type (SHA256, SHA512, ...)
		 * @return the signed string
		 * @see OpenSSL_add_all_digests();
		 */
		std::vector<unsigned char> rsa_sign_sha256(RSA* rsa
			, std::string const& msg
			, std::string digest_name);

		/** @brief hash (not encrypted)
		 *
		 * Create the hash of the specified string.
		 *
		 * @note the RSA object is no longer valid when function returns.
		 * @note use OpenSSL_add_all_algorithms() to provide all algorithms
		 *
		 * @param digest_name digest type (SHA256, SHA512, ...)
		 * @return the signed string
		 * @see OpenSSL_add_all_digests();
		 *
		 * example
		 * @code
		 auto dec = sign_string("hello, world", "sha256");
		 * @endcode
		 */
		std::vector<unsigned char> hash_string(std::string const& msg
			, std::string digest_name);

	}
}

#endif	

