/*
* The MIT License (MIT)
*
* Copyright (c) 2019 Sylko Olzscher
*
*/

#include <cyng/crypto/util/digest.h>
#include <cyng/crypto/util/factory.h>
#include <cyng/crypto/bio.h>
#include <boost/predef.h>
#include <iostream>
#include <stdio.h>

namespace cyng 
{
	namespace crypto
	{
		std::vector<unsigned char> rsa_sign_sha256(RSA* rsa
			, std::string const& msg
			, std::string digest_name)
		{
			//
			//	create a signing context
			//
			auto rsa_sign_ctxp = create_evp_ctx();

			//
			//	create a key store
			//
			auto pri_keyp = create_evp_pkey();

			//
			//	rsa will be freed when the pri_keyp is freed.
			//
			EVP_PKEY_assign_RSA(pri_keyp.get(), rsa);

			//
			//	initialize signing process
			//	ToDo: use EVP_get_digestbyname(argv[1]);
			//
			EVP_MD const* md = EVP_get_digestbyname(digest_name.c_str());
			if (EVP_DigestSignInit(rsa_sign_ctxp.get(), NULL, md, NULL, pri_keyp.get()) <= 0) {
				return std::vector<unsigned char>();
			}

			//
			//	update (could be done multiple times)
			//
			if (EVP_DigestSignUpdate(rsa_sign_ctxp.get(), msg.c_str(), msg.size()) <= 0) {
				return std::vector<unsigned char>();
			}

			//
			//	finalize signing process - get length
			//
			size_t enc_length = 0u;
			unsigned char md_value[EVP_MAX_MD_SIZE];
			if (EVP_DigestSignFinal(rsa_sign_ctxp.get(), md_value, &enc_length) <= 0) {
				return std::vector<unsigned char>();
			}

			//
			//	allocate buffer for encrypted message
			//
			//std::string enc;
			//enc.resize(enc_length, 0);
			//if (EVP_DigestSignFinal(rsa_sign_ctxp.get(), reinterpret_cast<unsigned char*>(const_cast<char*>(enc.c_str())), &enc_length) <= 0) {
			//	return std::vector<unsigned char>();
			//}
			return std::vector<unsigned char>(&md_value[0], &md_value[enc_length]);
		}

		std::vector<unsigned char> hash_string(std::string const& msg, std::string digest_name)
		{
			//
			//	get digest
			//
			EVP_MD const* md = EVP_get_digestbyname(digest_name.c_str());
			if (md == NULL)	return std::vector<unsigned char>();

			//
			//	create a signing context
			//
			auto sign_ctxp = create_evp_ctx();

			//
			//	initialize signing process
			//
			EVP_DigestInit_ex(sign_ctxp.get(), md, NULL);

			//
			//	update (could be done multiple times)
			//
			EVP_DigestUpdate(sign_ctxp.get(), msg.c_str(), msg.size());

			//
			//	finalize signing process - get length
			//
			unsigned int enc_length = 0;
			//if (EVP_DigestSignFinal(sign_ctxp.get(), NULL, &enc_length) <= 0) {
			//	return "";
			//}

			//
			//	allocate buffer for encrypted message
			//
			//std::string enc;
			//enc.resize(enc_length, 0);
			//if (EVP_DigestSignFinal(sign_ctxp.get(), reinterpret_cast<unsigned char*>(const_cast<char*>(enc.c_str())), &enc_length) <= 0) {
			//	return "";
			//}

			unsigned char md_value[EVP_MAX_MD_SIZE];
			EVP_DigestFinal_ex(sign_ctxp.get(), md_value, &enc_length);

			return std::vector<unsigned char>(&md_value[0], &md_value[enc_length]);
		}
	}
}
