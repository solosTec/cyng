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
		 * Generate a private key
		 */
		EVP_PKEY_ptr generate_private_key(int bits);

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

		/**
		 * Writing a private key to disk without a passphrase and encryption
		 */
		bool write_private_key(RSA*, const char* file_name);
		bool write_private_key(EVP_PKEY*, const char* file_name);

		/**
		 * Writing a public key to disk.
		 */
		bool write_public_key(RSA*, const char* file_name);

		/**
		 * Writing certificate to disk.
		 */
		bool write_certificate(X509* x509, const char* file_name);

		/**
		 * Generate a CA certificate and a private key
		 */
		bool generate_ca_cert_write(const char* priv_key
			, const char* cert_file
			, const char* pC	//	country
			, const char* pP	//	province
			, const char* pCity	//	city
			, const char* pOrg	//	organization
			, const char* pComm	//	common
			, long serial
			, long days);

		/**
		 * Generate a CA certificate from a private key
		 */
		bool generate_ca_cert_read(const char* priv_key
			, const char* cert_file
			, const char* pC	//	country
			, const char* pP	//	province
			, const char* pCity	//	city
			, const char* pOrg	//	organization
			, const char* pComm	//	common
			, long serial
			, long days);
	}
}

#endif	

