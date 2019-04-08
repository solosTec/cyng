/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include <cyng/crypto/aes.h>
#include <openssl/rand.h>
#include <boost/assert.hpp>

namespace cyng 
{
	namespace crypto
	{
		namespace aes
		{

			void randomize(aes_128_key& key)
			{
				RAND_bytes(key.key_.data(), key.key_.size());
			}

			void randomize(aes_192_key& key)
			{
				RAND_bytes(key.key_.data(), key.key_.size());
			}

			void randomize(aes_256_key& key)
			{
				RAND_bytes(key.key_.data(), key.key_.size());
			}

			void randomize(iv_t& iv)
			{
				RAND_bytes(iv.data(), iv.size());
			}

		}
	}
}
