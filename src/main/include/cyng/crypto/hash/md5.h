/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CRYPTO_HASH_MD5_H
#define CYNG_CRYPTO_HASH_MD5_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <string>
#include <openssl/md5.h>
#include <cyng/intrinsics/digest.h>
#include <cyng/intrinsics/buffer.h>

namespace cyng
{
	namespace crypto 
	{
		class md5
		{
		public:
			md5();
			
			bool update(std::string const&);
			bool update(const void* ptr, std::size_t length);
			
			digest_md5::value_type finalize();
			
		private:
			MD5_CTX ctx_;
		};
	}
	
	/**
	 * Calculate MD5 hash 
	 */
	crypto::digest_md5::value_type md5_hash(std::string const&);
	crypto::digest_md5::value_type md5_hash(buffer_t const&);
}

#endif
