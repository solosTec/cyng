/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Sylko Olzscher
 *
 */

#ifndef CYNG_CRYPTO_FACTORY_H
#define CYNG_CRYPTO_FACTORY_H

#include <cyng/crypto.h>

namespace cyng
{
	namespace crypto
	{
		/**
		 * create a bignum structure
		 */
		BN_ptr create_bn();

		/**
		 * create a RSA structure
		 */
		RSA_ptr create_rsa();
	}
}

#endif	

