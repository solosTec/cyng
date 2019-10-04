/*
* The MIT License (MIT)
*
* Copyright (c) 2019 Sylko Olzscher
*
*/

#include <cyng/crypto/util/generate_key.h>
#include <cyng/crypto/util/factory.h>
#include <cyng/crypto/util/x509.h>
#include <cyng/crypto/bio.h>
#include <cyng/crypto/util/print.h>
#include <boost/predef.h>
#include <openssl/x509v3.h>	//	X509V3_EXT_conf_nid
#include <openssl/asn1t.h>
#include <openssl/rsa.h>
#include <openssl/ssl.h>
#include <openssl/ossl_typ.h>	//	RSA
#include <openssl/evp.h>	//	EVP_PKEY
#include <iostream>
#include <stdio.h>

namespace cyng 
{
	namespace crypto
	{
		EVP_PKEY_ptr generate_private_key(int bits)
		{
			//
			//	To store private key algorithm-independent in memory.
			//
			auto evp_pkeyp = create_evp_pkey();
			if (!evp_pkeyp) return evp_pkeyp;

			//
			//	create the private key and assign it to the EVP_PKEY structure
			//
			auto bnp = create_bignum_rsa_f4();
			if (!bnp) return evp_pkeyp;

			auto rsap = create_rsa();
			if (!rsap) return evp_pkeyp;
			auto ret = RSA_generate_key_ex(rsap.get(), 2048, bnp.get(), NULL);
			if (ret != 1)	return evp_pkeyp;

			//
			//	The RSA structure will be automatically freed 
			//	when the EVP_PKEY structure is freed.
			//
			EVP_PKEY_assign_RSA(evp_pkeyp.get(), rsap.release());
			return evp_pkeyp;
		}

		bool generate_priv_pub_key_pair(const char* pub_pem
			, const char* priv_pem
			, int bits)
		{
			// 1. generate rsa key with exponent RSA_F4
			auto bnp = create_bignum_rsa_f4();
			if (!bnp) return false;

			auto rsap = create_rsa();
			if (!rsap) return false;
			auto ret = RSA_generate_key_ex(rsap.get(), bits, bnp.get(), NULL);
			if (ret != 1)	return false;

			// 2. save public key
			if (!write_public_key(rsap.get(), pub_pem)) {
				return false;
			}

			// 3. save private key
			return write_private_key(rsap.get(), priv_pem);
		}

		bool write_private_key(RSA* rsa, const char* file_name)
		{
			auto bio_privp = create_bio_file(file_name, "w+");
			if (!bio_privp) return false;

			auto ret = PEM_write_bio_RSAPrivateKey(
				bio_privp.get(), //	file
				rsa,	//	the private key
				NULL,	//	default cipher for encrypting the key on disk (EVP_des_ede3_cbc)
				NULL,	//	passphrase
				0,		//	length of passphrase
				NULL,	//	callback for requesting the password
				NULL);	//	data to pass to the callback
			return ret == 1;
		}

		bool write_private_key(EVP_PKEY* pkey, const char* file_name)
		{
			auto bio_privp = create_bio_file(file_name, "w+");
			if (!bio_privp) return false;

			auto ret = PEM_write_bio_PrivateKey(
				bio_privp.get(), //	file
				pkey,	//	the private key
				NULL,	//	default cipher for encrypting the key on disk (EVP_des_ede3_cbc)
				NULL,	//	passphrase
				0,		//	length of passphrase
				NULL,	//	callback for requesting the password
				NULL);	//	data to pass to the callback
			return ret == 1;
		}

		bool write_certificate(X509* x509, const char* file_name)
		{
			auto bio_privp = create_bio_file(file_name, "w+");
			if (!bio_privp) return false;
			auto ret = PEM_write_bio_X509(bio_privp.get(), x509);
			return ret == 1;
		}

		bool write_public_key(RSA* rsa, const char* file_name)
		{
			auto biop = create_bio_file(file_name, "w+");
			if (!biop) return false;
			auto ret = PEM_write_bio_RSAPublicKey(biop.get(), rsa);
			if (ret != 1)	return false;
			return true;
		}


		bool generate_ca_cert_write(const char* priv_key_file
			, const char* cert_file
			, const char* pC	//	country
			, const char* pST	//	province
			, const char* pL	//	city
			, const char* pO	//	organization
			, const char* pCN	//	common
			, long serial
			, long days)
		{
			//
			//	Create a EVP_PKEY struct with a private key
			//
			auto evp_pkeyp = generate_private_key(2048);

			//
			//	create a X509 structure to represent 
			//	the certificate in memory (Version 1.3)
			//
			auto x509p = create_x509(2);

			//
			//	set properties: serial
			//
			ASN1_INTEGER_set(X509_get_serialNumber(x509p.get()), serial);

			//
			//	set properties: time
			//
			X509_gmtime_adj(X509_get_notBefore(x509p.get()), 0);
			X509_gmtime_adj(X509_get_notAfter(x509p.get()), 60ul * 60ul * 24ul * days);

			//
			//	set properties: issuer name
			//
			auto x509_name = X509_get_subject_name(x509p.get());
			if (!x509_name) return false;
			add_entry_by_txt(x509_name, "C", pC);
			add_entry_by_txt(x509_name, "ST", pST);
			add_entry_by_txt(x509_name, "L", pL);
			add_entry_by_txt(x509_name, "O", pO);
			add_entry_by_txt(x509_name, "CN", pCN);

			X509_set_issuer_name(x509p.get(), x509_name);

			//
			//	signing the certificate with SHA1
			//
			X509_sign(x509p.get(), evp_pkeyp.get(), EVP_sha1());

#ifdef _DEBUG
			X509_print_fp(stdout, x509p.get());
#endif

			//
			//	write private key to disk
			//
			write_private_key(evp_pkeyp.get(), priv_key_file);

			//
			//	write certificate to disk
			//
			write_certificate(x509p.get(), cert_file);

			return true;
		}

		bool generate_ca_cert_read(const char* priv_key_file
			, const char* cert_file
			, const char* pC	//	country
			, const char* pST	//	province
			, const char* pL	//	city
			, const char* pO	//	organization
			, const char* pCN	//	common
			, long serial
			, long days)
		{
			//
			//	Create a EVP_PKEY struct with a private key
			//
			//auto evp_pkeyp = generate_private_key(2048);
			auto evp_pkeyp = load_CA_private_key(priv_key_file);
			if (!evp_pkeyp)	return false;

			//
			//	create a X509 structure to represent 
			//	the certificate in memory (Version 1.3)
			//
			auto x509p = create_x509(2);

			//
			//	set properties: serial
			//
			ASN1_INTEGER_set(X509_get_serialNumber(x509p.get()), serial);

			//
			//	set properties: time
			//
			X509_gmtime_adj(X509_get_notBefore(x509p.get()), 0);
			X509_gmtime_adj(X509_get_notAfter(x509p.get()), 60ul * 60ul * 24ul * days);

			//
			//	set properties: issuer name
			//
			auto x509_name = X509_get_subject_name(x509p.get());
			if (!x509_name) return false;
			add_entry_by_txt(x509_name, "C", pC);
			add_entry_by_txt(x509_name, "ST", pST);
			add_entry_by_txt(x509_name, "L", pL);
			add_entry_by_txt(x509_name, "O", pO);
			add_entry_by_txt(x509_name, "CN", pCN);

			X509_set_issuer_name(x509p.get(), x509_name);

			//
			//	signing the certificate with SHA1
			//
			X509_sign(x509p.get(), evp_pkeyp.get(), EVP_sha1());

#ifdef _DEBUG
			print_stdout_X509(x509p.get());
#endif

			//
			//	write private key to disk
			//
			//write_private_key(evp_pkeyp.get(), priv_key_file);

			//
			//	write certificate to disk
			//
			write_certificate(x509p.get(), cert_file);

			return true;
		}
	}
}
