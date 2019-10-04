
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include "test-crypto-003.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/crypto/util/generate_key.h>
#include <openssl/err.h>

namespace cyng 
{
	bool test_crypto_003()
	{
		OpenSSL_add_all_algorithms();
		ERR_load_crypto_strings();
		//OPENSSL_no_config();

		//
		//	create a private and a public key
		//
		bool const b = crypto::generate_priv_pub_key_pair("public.pem"
			, "private.pem"
			, 2048);

		if (b) {

			//
			//	use private key to generate a certificate
			//
			return crypto::generate_ca_cert_read("private.pem"
				, "mycert.pem"
				, "CH"	//	country
				, "LU"	//	province
				, "Lucerne"	//	city
				, "solosTec"	//	organization
				, "solostec.com"
				, 1
				, 365);
		}

		return b;
	}
	
}
