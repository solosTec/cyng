/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#define BOOST_TEST_MODULE CYNG
#include <boost/test/unit_test.hpp>
#include <cyng/cyng.h>

#include "test-core-001.h"
#include "test-core-002.h"
#include "test-core-003.h"
#include "test-core-004.h"

//	Start with:
//	./unit_test --report_level=detailed
//	./unit_test --run_test=CORE/core_001

BOOST_AUTO_TEST_SUITE(CORE)
BOOST_AUTO_TEST_CASE(core_001)
{
	using namespace cyng;
	BOOST_CHECK(test_core_001());
}
BOOST_AUTO_TEST_CASE(core_002)
{
	using namespace cyng;
	BOOST_CHECK(test_core_002());
}
BOOST_AUTO_TEST_CASE(core_003)
{
	using namespace cyng;
	BOOST_CHECK(test_core_003());
}
BOOST_AUTO_TEST_CASE(core_004)
{
	using namespace cyng;
	BOOST_CHECK(test_core_004());
}
BOOST_AUTO_TEST_SUITE_END()	//	CORE

#include "test-async-001.h"
#include "test-async-002.h"

BOOST_AUTO_TEST_SUITE(ASYNC)
BOOST_AUTO_TEST_CASE(async_001)
{
	using namespace cyng;
	BOOST_CHECK(test_async_001());
}
BOOST_AUTO_TEST_CASE(async_002)
{
	using namespace cyng;
	BOOST_CHECK(test_async_002());
}
BOOST_AUTO_TEST_SUITE_END()	//	ASYNC

#include "test-io-001.h"
#include "test-io-002.h"

BOOST_AUTO_TEST_SUITE(IO)
BOOST_AUTO_TEST_CASE(io_001)
{
	using namespace cyng;
	BOOST_CHECK(test_io_001());
}
BOOST_AUTO_TEST_CASE(io_002)
{
	using namespace cyng;
	BOOST_CHECK(test_io_002());
}
BOOST_AUTO_TEST_SUITE_END()	//	IO

#include "test-mail-001.h"

BOOST_AUTO_TEST_SUITE(MAIL)
BOOST_AUTO_TEST_CASE(mail_001)
{
	using namespace cyng;
	BOOST_CHECK(test_mail_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	MAIL

#include "test-log-001.h"

BOOST_AUTO_TEST_SUITE(LOG)
BOOST_AUTO_TEST_CASE(log_001)
{
	using namespace cyng;
	BOOST_CHECK(test_log_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	LOG

#include "test-store-001.h"
#include "test-store-002.h"

BOOST_AUTO_TEST_SUITE(STORE)
BOOST_AUTO_TEST_CASE(store_001)
{
	using namespace cyng;
	BOOST_CHECK(test_store_001());
}
BOOST_AUTO_TEST_CASE(store_002)
{
	using namespace cyng;
	BOOST_CHECK(test_store_002());
}
BOOST_AUTO_TEST_SUITE_END()	//	LOG

#include "test-vm-001.h"
// #include "test-store-002.h"

BOOST_AUTO_TEST_SUITE(VM)
BOOST_AUTO_TEST_CASE(vm_001)
{
	using namespace cyng;
	BOOST_CHECK(test_vm_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	VM

#include "test-sql-001.h"
// #include "test-sql-002.h"

BOOST_AUTO_TEST_SUITE(SQL)
BOOST_AUTO_TEST_CASE(sql_001)
{
	using namespace cyng;
	BOOST_CHECK(test_sql_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	SQL


#include "test-parser-001.h"

BOOST_AUTO_TEST_SUITE(PARSER)
BOOST_AUTO_TEST_CASE(parser_001)
{
	using namespace cyng;
	BOOST_CHECK(test_parser_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	PARSER


#include "test-json-001.h"

BOOST_AUTO_TEST_SUITE(JSON)
BOOST_AUTO_TEST_CASE(json_001)
{
	using namespace cyng;
	BOOST_CHECK(test_json_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	JSON

#include "test-crypto-001.h"

BOOST_AUTO_TEST_SUITE(CRYPTO)
BOOST_AUTO_TEST_CASE(crypto_001)
{
	using namespace cyng;
	BOOST_CHECK(test_crypto_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	CRYPTO
