/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#ifndef CYNG_CRYPTO_BASE64_H
#define CYNG_CRYPTO_BASE64_H

#include <stdio.h>
#include <stdint.h>
#include <string>

namespace cyng
{
	namespace crypto 
	{
		/**
		 * @return base64 encoded string
		 */
		std::string base64_encode(const char* in, int in_len);
		std::string base64_encode(std::string const&);
		
		/**
		 * @return base64 decoded string
		 */
		std::string base64_decode(const char* in, int in_len);
		std::string base64_decode(std::string const&);
	}
}

#endif
