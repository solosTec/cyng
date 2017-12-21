/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYY_CRYPTO_H
#define CYY_CRYPTO_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <openssl/bn.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/x509.h>
#include <memory>

namespace cyng 
{
	namespace crypto 
	{
		using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
		using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
		using EVP_KEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
		using BIO_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;
		using BIO_ptr_all = std::unique_ptr<BIO, decltype(&::BIO_free_all)>;
		using X509_REQ_ptr = std::unique_ptr<X509_REQ, decltype(&::X509_REQ_free)>;
		//using X509_NAME_ptr = std::unique_ptr<X509_NAME, decltype(&::???)>;
		
	}
}
#endif
