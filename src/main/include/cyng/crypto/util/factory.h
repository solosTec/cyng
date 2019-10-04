/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#ifndef CYNG_CRYPTO_FACTORY_H
#define CYNG_CRYPTO_FACTORY_H

#include <cyng/crypto.h>

namespace cyng
{
	namespace crypto
	{
		/**
		 * create a bignum structure
		 */
		BN_ptr create_bignum();
		BN_ptr create_bignum_rsa_f4();

		/**
		 * create a RSA structure
		 */
		RSA_ptr create_rsa();

		/**
		 * create a RSA key with the specified size (typically 1024 or 2048)
		 */
		RSA_ptr create_rsa_key(BIGNUM*, int bits);

		/**
		 * provide a X509 structure.
		 * Represents an x509 certificate in memory.
		 * A value of 2 stands for version 1.3, a value of 1 encodes
		 * version 1.2.
		 *
		 * @param v version (usually == 2)
		 */
		X509_ptr create_x509(long v);

		/**
		 * create a x509 request structure
		 * 
		 * @param v version (mostly 1)
		 */
		X509_REQ_ptr create_x509_request(int v);

		/**
		 * create a key store
		 */
		EVP_PKEY_ptr create_evp_pkey();

		/**
		 * create a signing context
		 */
		EVP_MD_CTX_ptr create_evp_ctx();

		/**
		 * create an SSL context
		 * The supported protocols are SSLv2, SSLv3, TLSv1, TLSv1.1 and TLSv1.2.
		 */
		SSL_CTX_ptr create_ssl_ctx_v23();
		SSL_CTX_ptr create_ssl_ctx_v23_client();
		SSL_CTX_ptr create_ssl_ctx_v23_server();

		/**
		 * Pointer is initialized with ASN1_TIME_new().
		 *
		 * @return pointer to ASN1_TIME
		 */
		ASN1_TIME_ptr create_asn1_time();
	}
}

#endif	

