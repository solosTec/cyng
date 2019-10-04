
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include "test-crypto-004.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/crypto/util/x509.h>

namespace cyng 
{
	bool test_crypto_004()
	{
		//
		//	precondition: generate a CA certificate
		//

		//openssl genrsa -out cakey.pem 2048
		//openssl req -new -days 365 -x509 -key cakey.pem -out cacert.pem -nodes -subj /C=CH/ST=LU/L=Lucerne/O=solosTec/OU=solosTec/CN=solsoTec/emailAddress=info@solsotec.com
		//openssl rsa -in cakey.pem -pubout -out ca_pub.key

		//
		//	generate a certification request
		//
		if (crypto::generate_x509_cert_request("CH"	//	country
			, "LU"	//	province
			, "LU"	//	city
			, "solosTec"	//	organization
			, "solostec.com"	//	common
			, "x509Req.pem"		//	output file
			, 2048)) {

			//
			//	sign the request
			//
			return crypto::sign_x509_with_CA("cacert.pem"	// CA certificate
				, "cakey.pem"	//	CA private key
				, "x509Req.pem"	//	see test_crypto_004()
				, "mySignedCert.pem"	//	output file
				, 365);

		}
		return false;
	}
	
}
