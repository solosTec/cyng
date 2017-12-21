/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/crypto/hash/md5.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse

namespace cyng
{
	namespace crypto 
	{
		md5::md5()
		: ctx_()
		{
			MD5_Init(&ctx_);
		}
		
		bool md5::update(std::string const& str)
		{
			return update(str.c_str(), str.length());
		}
		
		bool md5::update(const void* ptr, std::size_t length)
		{
			return MD5_Update(&ctx_, ptr, length) != 0;
		}
		
		digest_md5::value_type md5::finalize()
		{
			digest_md5::value_type d;
			MD5_Final(d.data(), &ctx_);
			OPENSSL_cleanse(&ctx_, sizeof(ctx_));
			return d;
		}
	}
	
	crypto::digest_md5::value_type md5_hash(std::string const& str)
	{
		crypto::md5 h;
		h.update(str);
		return h.finalize();
	}
	
	crypto::digest_md5::value_type md5_hash(buffer_t const& b)
	{
		crypto::md5 h;
		h.update(b.data(), b.size());
		return h.finalize();
	}
	
}
