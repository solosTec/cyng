/*
* The MIT License (MIT)
*
* Copyright (c) 2019 Sylko Olzscher
*
*/

#include <cyng/crypto/util/print.h>
#include <cyng/crypto/util/factory.h>
#include <cyng/crypto/util/x509.h>
#include <cyng/crypto/bio.h>
#include <boost/predef.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/x509.h>

namespace cyng 
{
	namespace crypto
	{
		bool print_stdout_public(EVP_PKEY* pkey)
		{
			if (pkey == nullptr)	return false;
			auto biop = create_bio_stdout();
			auto const r = EVP_PKEY_print_public(biop.get(), pkey, 0, nullptr);
			return r == 1;
		}

		bool print_stdout_private(EVP_PKEY* pkey)
		{
			if (pkey == nullptr)	return false;
			auto biop = create_bio_stdout();
			auto const r = EVP_PKEY_print_private(biop.get(), pkey, 0, nullptr);
			return r == 1;
		}

		bool print_stdout_params(EVP_PKEY* pkey)
		{
			if (pkey == nullptr)	return false;
			auto biop = create_bio_stdout();
			auto const r = EVP_PKEY_print_params(biop.get(), pkey, 0, nullptr);
			return r == 1;
		}

		bool print_stdout_X509(X509* x509)
		{
			if (x509 == nullptr)	return false;
			auto const r = X509_print_fp(stdout, x509);
			return r == 1;
		}

		bool print_stdout_RSA(RSA* rsa)
		{
			if (rsa == nullptr)	return false;
			auto const r = RSA_print_fp(stdout, rsa, 0);
			return r == 1;
		}

		bool dump_evp(const char* filename)
		{
			auto pkey = load_CA_private_key(filename);
			if (!pkey)	return false;
			print_stdout_public(pkey.get());
			print_stdout_private(pkey.get());
			print_stdout_params(pkey.get());
			return true;
		}

		bool dump_x509(const char* filename)
		{
			auto biop = create_bio_file(filename, "r");
			auto x509 = PEM_read_bio_X509(biop.get(), NULL, 0, NULL);
			if (x509 == nullptr)	return false;
			return print_stdout_X509(x509);
		}
	}
}
