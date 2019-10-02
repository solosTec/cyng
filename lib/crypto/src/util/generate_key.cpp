/*
* The MIT License (MIT)
*
* Copyright (c) 2019 Sylko Olzscher
*
*/

#include <cyng/crypto/util/generate_key.h>
#include <cyng/crypto/util/factory.h>
#include <cyng/crypto/bio.h>
#include <boost/predef.h>
#include <iostream>
#include <stdio.h>

namespace cyng 
{
	namespace crypto
	{
		bool generate_priv_pub_key_pair(const char* pub_pem
			, const char* priv_pem
			, int bits)
		{
			// 1. generate rsa key
			auto bnp = create_bn();
			
			//	set exponent
			auto ret = BN_set_word(bnp.get(), RSA_F4);
			if (ret != 1)	return false;

			auto rsap = create_rsa();
			ret = RSA_generate_key_ex(rsap.get(), bits, bnp.get(), NULL);
			if (ret != 1)	return false;

			// 2. save public key
			auto biop = create_bio_file(pub_pem, "w+");
			ret = PEM_write_bio_RSAPublicKey(biop.get(), rsap.get());
			if (ret != 1)	return false;

			// 3. save private key
			auto bio_privp = create_bio_file(priv_pem, "w+");
			ret = PEM_write_bio_RSAPrivateKey(bio_privp.get(), rsap.get(), NULL, NULL, 0, NULL, NULL);
			if (ret != 1)	return false;

			return true;
		}
	}
}
