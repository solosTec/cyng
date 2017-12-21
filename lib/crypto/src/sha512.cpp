/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/crypto/hash/sha512.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse

namespace cyng
{
	namespace crypto 
	{
		sha512::sha512()
		: ctx_()
		{
			SHA512_Init(&ctx_);
		}
		
		bool sha512::update(std::string const& str)
		{
			return update(str.c_str(), str.length());
		}
		
		bool sha512::update(const void* ptr, std::size_t length)
		{
			return SHA512_Update(&ctx_, ptr, length) != 0;
		}
		
		digest_sha512::value_type sha512::finalize()
		{
			digest_sha512::value_type d;
			SHA512_Final(d.data(), &ctx_);
			OPENSSL_cleanse(&ctx_, sizeof(ctx_));
			return d;
		}
	}
	
	crypto::digest_sha512::value_type sha512_hash(std::string const& str)
	{
		crypto::sha512 h;
		h.update(str);
		return h.finalize();
	}
	
	crypto::digest_sha512::value_type sha512_hash(buffer_t const& b)
	{
		crypto::sha512 h;
		h.update(b.data(), b.size());
		return h.finalize();
	}
}
