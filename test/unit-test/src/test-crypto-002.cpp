
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include "test-crypto-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/compatibility/general.h>
#include <cyng/crypto/aes.h>
#include <openssl/rand.h>

namespace cyng 
{
	bool test_crypto_002()
	{
		{
			//
			//	AES ECB
			//

			crypto::aes_128_key key;
			key.randomize();

			cyng::buffer_t inp{ 'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
			auto enc = crypto::aes::encrypt(inp, key);
			auto dec = crypto::aes::decrypt(enc, key);
			dec.at(13) = '\0';
			//std::cout << std::string(dec.begin(), dec.end()) << std::endl;
			//BOOST_CHECK_EQUAL(std::string(inp.begin(), inp.end()), std::string(dec.begin(), dec.end()));
		}

		{
			//
			//	AES CBC
			//

			crypto::aes_128_key key;
			key.randomize();

			crypto::aes::iv_t iv;
			BOOST_ASSERT(iv.size() == AES_BLOCK_SIZE);
			RAND_bytes(iv.data(), AES_BLOCK_SIZE);

			cyng::buffer_t inp{ 'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!' };
			auto enc = crypto::aes::encrypt(inp, key, iv);
			auto dec = crypto::aes::decrypt(enc, key, iv);

			dec.at(13) = '\0';
			//std::cout << std::string(dec.begin(), dec.end()) << std::endl;
			//BOOST_CHECK_EQUAL(std::string(inp.begin(), inp.end()), std::string(dec.begin(), dec.end()));
		}

		{
			//	init key
			crypto::aes_128_key key;
			key.key_ = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x11 };

			crypto::aes::iv_t iv{ 0x93, 0x15, 0x78, 0x56, 0x34, 0x12, 0x33, 0x03, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A };

			//	prepare input
			cyng::buffer_t inp = cyng::make_buffer({ 0x59, 0x23, 0xc9, 0x5a, 0xaa, 0x26, 0xd1, 0xb2, 0xe7, 0x49, 0x3b, 0x01, 0x3e, 0xc4, 0xa6, 0xf6 });

			auto dec = crypto::aes::decrypt(inp, key, iv);

			//	expected output:
			//	2F 2F 0C 14 27 04 85 02 04 6D 32 37 1F 15 02 FD
			BOOST_CHECK_EQUAL(dec.size(), 16);
			if (dec.size() == 16) {

				BOOST_CHECK_EQUAL(dec.at(0), 0x2F);
				BOOST_CHECK_EQUAL(dec.at(1), 0x2F);
				BOOST_CHECK_EQUAL(dec.at(15), 0xFD);
			}
		}


		return true;
	}
	
}
