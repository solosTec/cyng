/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#ifndef CYNG_CRYPTO_GENERATE_KEY_H
#define CYNG_CRYPTO_GENERATE_KEY_H

#include <cyng/crypto.h>

namespace cyng
{
	namespace crypto
	{
		/**
		 * generate a private and a public key pair
		 *
		 * @param pub_pem name of file for public key
		 * @param priv_pem name of file for private key
		 * @param bits (e.g. 2048)
		 */
		bool generate_priv_pub_key_pair(const char* pub_pem
			, const char* priv_pem
			, int bits);
	}
}

#endif	

