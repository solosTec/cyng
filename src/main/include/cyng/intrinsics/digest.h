/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CRYPTO_DIGEST_H
#define CYNG_CRYPTO_DIGEST_H

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
  #pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)


#include <cstdint>
#include <array>
#include <openssl/sha.h>
#include <openssl/md5.h>

namespace cyng
{
	namespace crypto 
	{
		/**
		 * MD5 digest with 128 bits / 16 bytes
		 */
		struct digest_md5
		{
			using value_type = std::array<std::uint8_t, MD5_DIGEST_LENGTH>;

			digest_md5();
			digest_md5(value_type const& v);
			digest_md5(value_type&& v);
			digest_md5(digest_md5 const& v);
			value_type data_;
		};
		
		/**
		 * SHA1 (Secure Hash Algorithm 1)  message digest with 160 bits / 20 bytes.
		 */
		struct digest_sha1
		{
			using value_type = std::array<std::uint8_t, SHA_DIGEST_LENGTH>;

			digest_sha1();
			digest_sha1(value_type const& v);
			digest_sha1(value_type&& v);
			digest_sha1(digest_sha1 const& v);
			value_type data_;
		};
		
		/**
		 * SHA256 digest with 256 bits / 32 bytes
		 */
		struct digest_sha256
		{
			using value_type = std::array<std::uint8_t, SHA256_DIGEST_LENGTH>;

			digest_sha256();
			digest_sha256(value_type const& v);
			digest_sha256(value_type&& v);
			digest_sha256(digest_sha256 const& v);
			value_type data_;
		};

		/**
		 * SHA512 digest with 512 bits / 64 bytes
		 */
		struct digest_sha512
		{
			using value_type = std::array<std::uint8_t, SHA512_DIGEST_LENGTH>;

			digest_sha512();
			digest_sha512(value_type const& v);
			digest_sha512(value_type&& v);
			digest_sha512(digest_sha512 const& v);
			value_type data_;
		};
	}

}

#include <functional>

namespace std
{
	template<>
	struct equal_to<cyng::crypto::digest_md5>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::digest_md5;
		using second_argument_type = cyng::crypto::digest_md5;

		bool operator()(cyng::crypto::digest_md5 const& c1, cyng::crypto::digest_md5 const& c2) const noexcept;
	};

	template<>
	struct equal_to<cyng::crypto::digest_sha1>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::digest_sha1;
		using second_argument_type = cyng::crypto::digest_sha1;

		bool operator()(cyng::crypto::digest_sha1 const& c1, cyng::crypto::digest_sha1 const& c2) const noexcept;
	};

	template<>
	struct equal_to<cyng::crypto::digest_sha256>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::digest_sha256;
		using second_argument_type = cyng::crypto::digest_sha256;

		bool operator()(cyng::crypto::digest_sha256 const& c1, cyng::crypto::digest_sha256 const& c2) const noexcept;
	};

	template<>
	struct equal_to<cyng::crypto::digest_sha512>
	{
		using result_type = bool;
		using first_argument_type = cyng::crypto::digest_sha512;
		using second_argument_type = cyng::crypto::digest_sha512;

		bool operator()(cyng::crypto::digest_sha512 const& c1, cyng::crypto::digest_sha512 const& c2) const noexcept;
	};
}

#endif	//	CYNG_CRYPTO_DIGEST_H
