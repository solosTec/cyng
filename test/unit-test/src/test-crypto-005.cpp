
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include "test-crypto-005.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/crypto/util/digest.h>
#include <cyng/crypto/util/print.h>
#include <openssl/err.h>

namespace cyng 
{
	bool test_crypto_005()
	{

		auto dec = crypto::hash_string("hello, world", "sha256");

		//
		//	result is: 09ca7e4eaa6e8ae9c7d261167129184883644d07dfba7cbfbc4c8a2e08360d5b
		//
		BOOST_CHECK_EQUAL(dec.size(), 32);

		BOOST_CHECK_EQUAL(dec.at(0), 0x09);
		BOOST_CHECK_EQUAL(dec.at(1), 0xca);
		BOOST_CHECK_EQUAL(dec.at(31), 0x5b);

		OpenSSL_add_all_algorithms();
		ERR_load_crypto_strings();
		//OPENSSL_no_config();

		//crypto::dump_evp("private.pem");
		//crypto::dump_evp("cakey.pem");
		crypto::dump_x509("mySignedCert.pem");

		return true;
	}
	
}
