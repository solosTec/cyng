/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include <cyng/crypto/x509.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse
#include <boost/predef.h>
#include <iostream>
#include <stdio.h>

namespace cyng 
{
	namespace crypto
	{
		EVP_PKEY * generate_key()
		{
			/* Allocate memory for the EVP_PKEY structure. */
			EVP_PKEY * pkey = EVP_PKEY_new();
			if (!pkey)
			{
				std::cerr << "Unable to create EVP_PKEY structure." << std::endl;
				return NULL;
			}
#if OPENSSL_VERSION_NUMBER >= 0x0090800fL
    
            RSA  *rsa = RSA_new();
            if (rsa == NULL) {
                return NULL;
            }

            BIGNUM * e = BN_new();
            if (e == NULL) {
                RSA_free(rsa);
                return NULL;
            }

            if (BN_set_word(e, RSA_F4) == 0) {
                BN_free(e);
                RSA_free(rsa);
                return NULL;
            }
            
            if (RSA_generate_key_ex(rsa, 2048, e, NULL) == 0) {
                BN_free(e);
                RSA_free(rsa);
                return NULL;
            }

            BN_free(e);            
#else
			/* Generate the RSA key and assign it to pkey. */
			RSA * rsa = RSA_generate_key(2048, RSA_F4, NULL, NULL);
#endif
			if (!EVP_PKEY_assign_RSA(pkey, rsa))
			{
				std::cerr << "Unable to generate 2048-bit RSA key." << std::endl;
				EVP_PKEY_free(pkey);
				return NULL;
			}

			/* The key has been generated, return it. */
			return pkey;
		}

		X509 * generate_x509(EVP_PKEY * pkey
			, long seconds
			, const char* pC
			, const char* pO
			, const char* pCN)
		{
			/* Allocate memory for the X509 structure. */
			X509 * x509 = X509_new();
			if (!x509)
			{
				std::cerr << "Unable to create X509 structure." << std::endl;
				return NULL;
			}

			/* Set the serial number. */
			ASN1_INTEGER_set(X509_get_serialNumber(x509), 1);

			/* This certificate is valid from now until exactly one year from now. */
			X509_gmtime_adj(X509_get_notBefore(x509), 0);
			//X509_gmtime_adj(X509_get_notAfter(x509), 31536000L);	//	seconds
			X509_gmtime_adj(X509_get_notAfter(x509), seconds);	//	seconds

			/* Set the public key for our certificate. */
			X509_set_pubkey(x509, pkey);

			/* We want to copy the subject name to the issuer name. */
			X509_NAME * name = X509_get_subject_name(x509);

			/* Set the country code and common name. */
			//X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)"CA", -1, -1, 0);
			//X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)"MyCompany", -1, -1, 0);
			//X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)"localhost", -1, -1, 0);
			::X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char *)pC, -1, -1, 0);
			::X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char *)pO, -1, -1, 0);
			::X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char *)pCN, -1, -1, 0);

			/* Now set the issuer name. */
			X509_set_issuer_name(x509, name);

			/* Actually sign the certificate with our key. */
			if (!X509_sign(x509, pkey, EVP_sha1()))
			{
				std::cerr << "Error signing certificate." << std::endl;
				X509_free(x509);
				return NULL;
			}

			return x509;
		}

		bool write_to_disk(EVP_PKEY * pkey, X509 * x509)
		{
            //  open the PEM file for writing the key to disk
#if BOOST_OS_WINDOWS
			FILE * pkey_file = nullptr;
            auto err = fopen_s(&pkey_file, "key.pem", "wb");
			if (err != 0)
			{
				std::cerr << "Unable to open \"key.pem\" for writing." << std::endl;
				return false;
			}
#else
            FILE * pkey_file = fopen("key.pem", "wb");
            if (pkey_file == nullptr)
            {
                std::cerr << "Unable to open \"key.pem\" for writing." << std::endl;
                return false;
            }

#endif
            //  Write the key to disk
			bool ret = ::PEM_write_PrivateKey(pkey_file, pkey, NULL, NULL, 0, NULL, NULL);
			fclose(pkey_file);

			if (!ret)
			{
				std::cerr << "Unable to write private key to disk." << std::endl;
				return false;
			}

#if BOOST_OS_WINDOWS
            /* Open the PEM file for writing the certificate to disk. */
			FILE * x509_file;
			err = ::fopen_s(&x509_file, "cert.pem", "wb");
			if (err != 0)
			{
				std::cerr << "Unable to open \"cert.pem\" for writing." << std::endl;
				return false;
			}
#else
            //  open the PEM file for writing the certificate to disk
            FILE * x509_file = fopen("cert.pem", "wb");
            if (x509_file == nullptr)
            {
                std::cerr << "Unable to open \"cert.pem\" for writing." << std::endl;
                return false;
            }
#endif

            //  Write the certificate to disk
			ret = ::PEM_write_X509(x509_file, x509);
			fclose(x509_file);

			if (!ret)
			{
				std::cerr << "Unable to write certificate to disk." << std::endl;
				return false;
			}

			return true;
		}

	}
}
