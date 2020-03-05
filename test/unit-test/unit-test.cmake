# 
#	reset 
#
set (unit_test)

set (unit_test_cpp
	test/unit-test/src/main.cpp
	test/unit-test/src/test-log-001.cpp
	test/unit-test/src/test-vm-001.cpp
	test/unit-test/src/test-sql-001.cpp
	test/unit-test/src/test-json-001.cpp
	test/unit-test/src/test-xml-001.cpp
	test/unit-test/src/test-chrono-001.cpp
	test/unit-test/src/test-sys-001.cpp
	test/unit-test/src/test-csv-001.cpp
)
    
set (unit_test_h
    src/main/include/cyng/cyng.h
	test/unit-test/src/test-log-001.h
	test/unit-test/src/test-vm-001.h
	test/unit-test/src/test-sql-001.h
	test/unit-test/src/test-json-001.h
	test/unit-test/src/test-xml-001.h
	test/unit-test/src/test-chrono-001.h
	test/unit-test/src/test-sys-001.h
	test/unit-test/src/test-csv-001.h
)

set (unit_test_core
	test/unit-test/src/test-core-001.cpp
	test/unit-test/src/test-core-002.cpp
	test/unit-test/src/test-core-003.cpp
	test/unit-test/src/test-core-004.cpp
	test/unit-test/src/test-core-005.cpp
	test/unit-test/src/test-core-006.cpp
	test/unit-test/src/test-core-001.h
	test/unit-test/src/test-core-002.h
	test/unit-test/src/test-core-003.h
	test/unit-test/src/test-core-004.h
	test/unit-test/src/test-core-005.h
	test/unit-test/src/test-core-006.h
)

set (unit_test_async
	test/unit-test/src/test-async-001.cpp
	test/unit-test/src/test-async-002.cpp
	test/unit-test/src/test-async-003.cpp
	test/unit-test/src/test-async-004.cpp
	test/unit-test/src/test-async-005.cpp
	test/unit-test/src/test-async-006.cpp
	test/unit-test/src/test-async-007.cpp
	test/unit-test/src/test-async-001.h
	test/unit-test/src/test-async-002.h
	test/unit-test/src/test-async-003.h
	test/unit-test/src/test-async-004.h
	test/unit-test/src/test-async-005.h
	test/unit-test/src/test-async-006.h
	test/unit-test/src/test-async-007.h
)

set (unit_test_io
	test/unit-test/src/test-io-001.cpp
	test/unit-test/src/test-io-002.cpp
	test/unit-test/src/test-io-003.cpp
	test/unit-test/src/test-io-001.h
	test/unit-test/src/test-io-002.h
	test/unit-test/src/test-io-003.h
)

set (unit_test_store
	test/unit-test/src/test-store-001.cpp
    test/unit-test/src/test-store-002.cpp
    test/unit-test/src/test-store-003.cpp
    test/unit-test/src/test-store-004.cpp
    test/unit-test/src/test-store-005.cpp
    test/unit-test/src/test-store-006.cpp
    test/unit-test/src/test-store-007.cpp
	test/unit-test/src/test-store-001.h
	test/unit-test/src/test-store-002.h
	test/unit-test/src/test-store-003.h
	test/unit-test/src/test-store-004.h
	test/unit-test/src/test-store-005.h
	test/unit-test/src/test-store-006.h
	test/unit-test/src/test-store-007.h
)

set (unit_test_parser
	test/unit-test/src/test-parser-001.cpp
	test/unit-test/src/test-parser-002.cpp
	test/unit-test/src/test-parser-001.h
	test/unit-test/src/test-parser-002.h
)


source_group("core" FILES ${unit_test_core})
source_group("async" FILES ${unit_test_async})
source_group("io" FILES ${unit_test_io})
source_group("store" FILES ${unit_test_store})
source_group("parser" FILES ${unit_test_parser})

# define the unit test
set (unit_test
  ${unit_test_cpp}
  ${unit_test_h}
  ${unit_test_core}
  ${unit_test_async}
  ${unit_test_io}
  ${unit_test_store}
  ${unit_test_parser}
)

#
# MySQL
#
if (CYNG_ODBC_INSTALLED)
	message(STATUS "** unit test contains ODBC")

	set (unit_test_odbc
		test/unit-test/src/test-db-002.h
		test/unit-test/src/test-db-002.cpp
	)
	list(APPEND unit_test ${unit_test_odbc})

endif (CYNG_ODBC_INSTALLED)

#
# SQLite3
#
if (CYNG_SQLITE3_INSTALLED)
	message(STATUS "** unit test contains SQLite3")
	set (unit_test_sqlite3
		test/unit-test/src/test-db-001.h
		test/unit-test/src/test-db-001.cpp
	)
	list(APPEND unit_test ${unit_test_sqlite3})
endif (CYNG_SQLITE3_INSTALLED)
