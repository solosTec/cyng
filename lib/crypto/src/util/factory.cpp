/*
* The MIT License (MIT)
*
* Copyright (c) 2019 Sylko Olzscher
*
*/

#include <cyng/crypto/util/factory.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse
#include <boost/predef.h>
#include <iostream>
#include <stdio.h>

namespace cyng 
{
	namespace crypto
	{
		BN_ptr create_bn()
		{
			return BN_ptr(BN_new(), BN_free);
		}

		RSA_ptr create_rsa()
		{
			return RSA_ptr(RSA_new(), RSA_free);
		}

	}
}
