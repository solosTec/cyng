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
		 * Generates a X.509 Certificate Request
		 */
		bool generate_x509_cert_request(const char* pC	//	country
			, const char* pP	//	province
			, const char* pCity	//	city
			, const char* pOrg	//	organization
			, const char* pComm	//	common
			, const char* filename	//	name of output PEM file
			, int bits);
		 
		/**
		 * load a CA from the specified file
		 */
		X509_ptr load_CA(const char* filename);

		/**
		 * Read a private key and create an RSA pointer.
		 */
		RSA_ptr load_private_key(const char* filename);

		/**
		 * load (CA) private key (without password)
		 */
		EVP_PKEY_ptr load_CA_private_key(const char* filename);

		/**
		 * load X509 certificate request (without password)
		 */
		X509_REQ_ptr load_x509_request(const char* filename);

		/**
		 * Sign a certification request
		 *
		 * To generate ca certificate use the following commands:
		 *
		 * @code
			openssl genrsa -out cakey.pem 2048
			openssl req -new -days 365 -x509 -key cakey.pem -out cacert.pem -nodes -subj /C=CH/ST=LU/L=Lucerne/O=solosTec/OU=solosTec/CN=solsoTec/emailAddress=info@solsotec.com
			openssl rsa -in cakey.pem -pubout -out ca_pub.key
		 * @endcode
		 */
		bool sign_x509_with_CA(const char* caFile //  cacert.pem
			, const char* caPrivateKeyFile //	cakey.pem
			, const char* x509ReqFile	//	x509Req.pem
			, const char* szUserCert	//	cert.pem
			, long days);

		/**
		 * helper function to populate a x509_name structure.
		 *
		 * @param subject entry name
		 * @param txt text/value of this entry
		 */
		bool add_entry_by_txt(X509_NAME*, const char* subject, const char* txt);

		 /**
		 * Generates a 2048-bit RSA key. 
		 */
		//EVP_PKEY * generate_key();

		/**
		 * Generates a self-signed x509 certificate. 
		 * 
		 * @param seconds validity in seconds
		 */
		//X509 * generate_x509(EVP_PKEY * pkey
		//	, long seconds
		//	, const char* pC 
		//	, const char* pO
		//	, const char* pCN);

		//bool write_to_disk(EVP_PKEY * pkey, X509 * x509);


	}
}

#endif	

