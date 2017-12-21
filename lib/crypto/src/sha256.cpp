/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/crypto/hash/sha256.h>
#include <openssl/crypto.h>	//	OPENSSL_cleanse

namespace cyng
{
	namespace crypto
	{
		sha256::sha256()
			: ctx_()
		{
			SHA256_Init(&ctx_);
		}

		bool sha256::update(std::string const& str)
		{
			return update(str.c_str(), str.length());
		}

		bool sha256::update(const void* ptr, std::size_t length)
		{
			return SHA256_Update(&ctx_, ptr, length) != 0;
		}

		digest_sha256::value_type sha256::finalize()
		{
			digest_sha256::value_type d;
			SHA256_Final(d.data(), &ctx_);
			OPENSSL_cleanse(&ctx_, sizeof(ctx_));
			return d;
		}
	}

	crypto::digest_sha256::value_type sha256_hash(std::string const& str)
	{
		crypto::sha256 h;
		h.update(str);
		return h.finalize();
	}

	crypto::digest_sha256::value_type sha256_hash(buffer_t const& b)
	{
		crypto::sha256 h;
		h.update(b.data(), b.size());
		return h.finalize();
	}
}
