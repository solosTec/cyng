/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYY_CRYPTO_SHA512_H
#define CYY_CRYPTO_SHA512_H

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
		class sha512
		{
		public:
			sha512();
			
			bool update(std::string const&);
			bool update(const void* ptr, std::size_t length);
			
			digest_sha512::value_type finalize();
			
		private:
			SHA512_CTX ctx_;
		};
	}
	
	/**
	 * Calculate SHA512 hash 
	 */
	crypto::digest_sha512::value_type sha512_hash(std::string const&);
	crypto::digest_sha512::value_type sha512_hash(buffer_t const&);
}

#endif	//	CYY_CRYPTO_SHA512_H

