/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/crypto/hash/sha1.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse

namespace cyng
{
	namespace crypto 
	{
		sha1::sha1()
		: ctx_()
		{
			SHA1_Init(&ctx_);
		}
		
		bool sha1::update(std::string const& str)
		{
			return update(str.c_str(), str.length());
		}
		
		bool sha1::update(const void* ptr, std::size_t length)
		{
			return SHA1_Update(&ctx_, ptr, length) != 0;
		}
		
		digest_sha1::value_type sha1::finalize()
		{
			digest_sha1::value_type d;
			SHA1_Final(d.data(), &ctx_);
			OPENSSL_cleanse(&ctx_, sizeof(ctx_));
			return d;
		}
	}
	
	crypto::digest_sha1::value_type sha1_hash(std::string const& str)
	{
		crypto::sha1 h;
		h.update(str);
		return h.finalize();
	}
	
	crypto::digest_sha1::value_type sha1_hash(buffer_t const& b)
	{
		crypto::sha1 h;
		h.update(b.data(), b.size());
		return h.finalize();
	}	
}
