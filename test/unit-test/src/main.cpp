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
#include "test-core-005.h"
#include "test-core-006.h"

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
BOOST_AUTO_TEST_CASE(core_005)
{
	using namespace cyng;
	BOOST_CHECK(test_core_005());
}
BOOST_AUTO_TEST_CASE(core_006)
{
	using namespace cyng;
	BOOST_CHECK(test_core_006());
}
BOOST_AUTO_TEST_SUITE_END()	//	CORE

#include "test-async-001.h"
#include "test-async-002.h"
#include "test-async-003.h"
#include "test-async-004.h"
#include "test-async-005.h"
#include "test-async-006.h"

//	Start with:
//	./unit_test --run_test=ASYNC/async_001

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
BOOST_AUTO_TEST_CASE(async_003)
{
	//
	//	granularity test
	//
	using namespace cyng;
	BOOST_CHECK(test_async_003());
}
BOOST_AUTO_TEST_CASE(async_004)
{
	//
	//	nested strand test
	//
	using namespace cyng;
	BOOST_CHECK(test_async_004());
}
BOOST_AUTO_TEST_CASE(async_005)
{
	//
	//	stress test
	//
	using namespace cyng;
	BOOST_CHECK(test_async_005());
}
BOOST_AUTO_TEST_CASE(async_006)
{
	//
	//	stress test
	//
	using namespace cyng;
	BOOST_CHECK(test_async_006());
}
BOOST_AUTO_TEST_SUITE_END()	//	ASYNC

#include "test-io-001.h"
#include "test-io-002.h"
#include "test-io-003.h"

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
BOOST_AUTO_TEST_CASE(io_003)
{
	using namespace cyng;
	BOOST_CHECK(test_io_003());
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
#include "test-store-003.h"
#include "test-store-004.h"

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
BOOST_AUTO_TEST_CASE(store_003)
{
	using namespace cyng;
	BOOST_CHECK(test_store_003());
}
BOOST_AUTO_TEST_CASE(store_004)
{
	using namespace cyng;
	BOOST_CHECK(test_store_004());
}
BOOST_AUTO_TEST_SUITE_END()	//	STORE

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


#if	CYNG_SQLITE3_INSTALLED
#include "test-db-001.h"
#endif
#if	defined(CYNG_ODBC_INSTALLED)
#include "test-db-002.h"
#endif

BOOST_AUTO_TEST_SUITE(DB)
BOOST_AUTO_TEST_CASE(db_001)
{
	using namespace cyng;
#if	CYNG_SQLITE3_INSTALLED
	BOOST_CHECK(test_db_001());
#endif
}
BOOST_AUTO_TEST_CASE(db_002)
{
	using namespace cyng;
#if	defined(CYNG_ODBC_INSTALLED)
	BOOST_CHECK(test_db_002());
#endif
}
BOOST_AUTO_TEST_SUITE_END()	//	DB

#include "test-parser-001.h"
#include "test-parser-002.h"

BOOST_AUTO_TEST_SUITE(PARSER)
BOOST_AUTO_TEST_CASE(parser_001)
{
	using namespace cyng;
	BOOST_CHECK(test_parser_001());
}
BOOST_AUTO_TEST_CASE(parser_002)
{
	using namespace cyng;
	BOOST_CHECK(test_parser_002());
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

#ifdef	CYNG_PUGIXML_INSTALLED
#include "test-xml-001.h"
#endif

BOOST_AUTO_TEST_SUITE(XML)
BOOST_AUTO_TEST_CASE(xml_001)
{
	using namespace cyng;
#ifdef	CYNG_PUGIXML_INSTALLED
	BOOST_CHECK(test_xml_001());
#endif
}
BOOST_AUTO_TEST_SUITE_END()	//	XML

#include "test-chrono-001.h"
BOOST_AUTO_TEST_SUITE(CHRONO)
BOOST_AUTO_TEST_CASE(chrono_001)
{
	using namespace cyng;
	BOOST_CHECK(test_chrono_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	CHRONO

#include "test-sys-001.h"

BOOST_AUTO_TEST_SUITE(SYS)
BOOST_AUTO_TEST_CASE(sys_001)
{
	using namespace cyng;
	BOOST_CHECK(test_sys_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	SYS


#include "test-csv-001.h"

BOOST_AUTO_TEST_SUITE(CSV)
BOOST_AUTO_TEST_CASE(csv_001)
{
	using namespace cyng;
	BOOST_CHECK(test_csv_001());
}
BOOST_AUTO_TEST_SUITE_END()	//	CSV
