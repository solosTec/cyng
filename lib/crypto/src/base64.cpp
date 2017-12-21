/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/crypto/base64.h>
#include <cyng/crypto.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <openssl/buffer.h>
#include <openssl/evp.h>
#include <openssl/bio.h>

namespace cyng
{
	namespace crypto 
	{
		std::string base64_encode(const char* in, int len)
		{
// 			std::cout << "base64_encode: " << std::string(in, len) << std::endl;
			//	get the base64 BIO method
			BIO_ptr_all b64(::BIO_new(BIO_f_base64()), ::BIO_free_all);
			BIO *bmem = ::BIO_new(BIO_s_mem());
			bmem = ::BIO_push(b64.get(), bmem);
			
			//	Ignore newlines - write everything in one line
			::BIO_set_flags(bmem, BIO_FLAGS_BASE64_NO_NL);
			
			::BIO_write(b64.get(), in, len);
			
			//	no more data is to be encoded
			BIO_flush(b64.get());
			
			BUF_MEM *bptr;
			::BIO_get_mem_ptr(b64.get(), &bptr);
			
// 			std::string str(bptr->data, bptr->length);
// 			std::cout << "b64-encode: " << bptr->length << " - " << str << std::endl;
			
			return std::string(bptr->data, bptr->length);
		}
		
		std::string base64_encode(std::string const& str)
		{
			return base64_encode(str.c_str(), str.length());
		}
		
		std::string base64_decode(const char* in, int len)
		{
// 			std::cout << "base64_decode: " << std::string(in, len) << std::endl;
			//	get the base64 BIO method
			BIO_ptr_all b64(::BIO_new(BIO_f_base64()), ::BIO_free_all);
			
			//	Don't require trailing newlines
			::BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL); 
 			BIO *bmem = ::BIO_new_mem_buf(in, len);
 			::BIO_push(b64.get(), bmem);
			
			std::stringstream result;
			std::vector<char> buf(1024);
			while (auto nread = ::BIO_read(b64.get(), buf.data(), buf.size()))
			{
// 				std::cout << "nread: " << nread << std::endl;				
				result.write(buf.data(), nread);
			}
    
// 			std::cout << "result: " << result.str() << std::endl;
			return result.str();
 			
		}
		
		std::string base64_decode(std::string const& str)
		{
			return base64_decode(str.c_str(), str.length());
		}
	}
}

