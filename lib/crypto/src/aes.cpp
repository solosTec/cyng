/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include <cyng/crypto/aes.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse
#include <boost/assert.hpp>

namespace cyng 
{
	namespace crypto
	{
		void encrypt(buffer_t& out, buffer_t const& inp, aes_256_key const& key)
		{
            BOOST_ASSERT_MSG(inp.size() == 16, "wrong AES block size");
			out.resize(inp.size());

			::AES_KEY enc_key;
			::AES_set_encrypt_key(key.aes_key_.data(), 256, &enc_key);
			::AES_encrypt((const unsigned char*)inp.data(), (unsigned char*)out.data(), &enc_key);
		}

		void decrypt(buffer_t& out, buffer_t const& inp, aes_256_key const& key)
		{
            BOOST_ASSERT_MSG(inp.size() == 16, "wrong AES block size");
            out.resize(inp.size());

			::AES_KEY dec_key;
			::AES_set_decrypt_key(key.aes_key_.data(), 256, &dec_key);
			::AES_decrypt((const unsigned char*)inp.data(), (unsigned char*)out.data(), &dec_key);

		}
	}
}
