
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include "test-crypto-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/compatibility/general.h>
#include <cyng/crypto/base64.h>


namespace cyng 
{
#if defined(_CYNG_CPP_SUPPORT_N3642)
    using namespace std::string_literals;
#endif

	bool test_crypto_001()
	{
		{
// 			auto r0 = "grrrr shebangit!"s;
            auto r0 = CYNG_STRING_LITERAL("grrrr shebangit!");
			auto r1 = crypto::base64_encode(r0);
			BOOST_CHECK_EQUAL(r1, "Z3JycnIgc2hlYmFuZ2l0IQ==");
			auto r2 = crypto::base64_decode(r1);
			BOOST_CHECK_EQUAL(r0, r2);
		}
		
		{
			auto r0 = CYNG_STRING_LITERAL("demo:demo");
	// 		auto r0 = std::string("demo:demo");
			auto r1 = crypto::base64_encode(r0);
			BOOST_CHECK_EQUAL(r1, "ZGVtbzpkZW1v");
			auto r2 = crypto::base64_decode(r1);
			BOOST_CHECK_EQUAL(r0, r2);
		}
		
		return true;
	}
	
}
