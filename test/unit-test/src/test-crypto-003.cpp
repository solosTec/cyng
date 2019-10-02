
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2019 Sylko Olzscher 
 * 
 */ 

#include "test-crypto-003.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/crypto/util/generate_key.h>

namespace cyng 
{
	bool test_crypto_003()
	{
		return crypto::generate_priv_pub_key_pair("public.pem"
			, "private.pem"
			, 2048);
	}
	
}
