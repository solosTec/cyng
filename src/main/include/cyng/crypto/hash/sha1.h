/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CRYPTO_SHA1_H
#define CYNG_CRYPTO_SHA1_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <string>
#include <openssl/sha.h>
#include <cyng/intrinsics/digest.h>
#include <cyng/intrinsics/buffer.h>

namespace cyng
{
	namespace crypto 
	{
		class sha1
		{
		public:
			sha1();
			
			bool update(std::string const&);
			bool update(const void* ptr, std::size_t length);
			
			digest_sha1::value_type finalize();
			
		private:
			SHA_CTX ctx_;
		};
	}
	
	/**
	 * Calculate SHA1 hash 
	 */
	crypto::digest_sha1::value_type sha1_hash(std::string const&);
	crypto::digest_sha1::value_type sha1_hash(buffer_t const&);
}

#endif	//	CYNG_CRYPTO_SHA1_H

