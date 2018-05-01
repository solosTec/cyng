/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Sylko Olzscher
 *
 */

#ifndef CYNG_CRYPTO_X509_H
#define CYNG_CRYPTO_X509_H

#include <cyng/crypto.h>

namespace cyng
{
	namespace crypto
	{
		/**
		 * Generates a 2048-bit RSA key. 
		 */
		EVP_PKEY * generate_key();

		/**
		 * Generates a self-signed x509 certificate. 
		 * 
		 * @param seconds validity in seconds
		 */
		X509 * generate_x509(EVP_PKEY * pkey
			, long seconds
			, const char* pC 
			, const char* pO
			, const char* pCN);

		bool write_to_disk(EVP_PKEY * pkey, X509 * x509);

	}
}

#endif	

