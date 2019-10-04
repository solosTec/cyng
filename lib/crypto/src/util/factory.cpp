/*
* The MIT License (MIT)
*
* Copyright (c) 2019 Sylko Olzscher
*
*/

#include <cyng/crypto/util/factory.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse
#include <boost/predef.h>
#include <boost/assert.hpp>

namespace cyng 
{
	namespace crypto
	{
		BN_ptr create_bignum()
		{
			return BN_ptr(BN_new(), BN_free);
		}

		BN_ptr create_bignum_rsa_f4()
		{
			auto p = create_bignum();
			auto ret = BN_set_word(p.get(), RSA_F4);
			BOOST_ASSERT(ret == 1);
			return p;
		}

		RSA_ptr create_rsa()
		{
			return RSA_ptr(RSA_new(), RSA_free);
		}

		RSA_ptr create_rsa_key(BIGNUM* bnp, int bits)
		{
			auto p = create_rsa();
			auto ret = RSA_generate_key_ex(p.get(), bits, bnp, NULL);
			BOOST_ASSERT(ret == 1);
			return p;
		}

		X509_ptr create_x509(long v)
		{
			auto p = X509_ptr(X509_new(), X509_free);
			X509_set_version(p.get(), v);
			return p;
		}

		X509_REQ_ptr create_x509_request(int v)
		{
			auto p = X509_REQ_ptr(X509_REQ_new(), X509_REQ_free);
			auto ret = X509_REQ_set_version(p.get(), v);
			BOOST_ASSERT(ret == 1);
			return p;
		}

		EVP_PKEY_ptr create_evp_pkey()
		{
			return EVP_PKEY_ptr(EVP_PKEY_new(), EVP_PKEY_free);
		}

		EVP_MD_CTX_ptr create_evp_ctx()
		{
			return EVP_MD_CTX_ptr(EVP_MD_CTX_new(), EVP_MD_CTX_free);
		}


		SSL_CTX_ptr create_ssl_ctx_v23()
		{
			return SSL_CTX_ptr(SSL_CTX_new(SSLv23_method()), SSL_CTX_free);
		}

		SSL_CTX_ptr create_ssl_ctx_v23_client()
		{
			return SSL_CTX_ptr(SSL_CTX_new(SSLv23_client_method()), SSL_CTX_free);
		}

		SSL_CTX_ptr create_ssl_ctx_v23_server()
		{
			return SSL_CTX_ptr(SSL_CTX_new(SSLv23_server_method()), SSL_CTX_free);
		}

		ASN1_TIME_ptr create_asn1_time()
		{
			return ASN1_TIME_ptr(ASN1_TIME_new(), ASN1_STRING_free);
		}

	}
}
