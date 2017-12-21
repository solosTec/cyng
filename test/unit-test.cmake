# 
#	reset 
#
set (unit_test)

set (unit_test_cpp
	test/unit-test/main.cpp
	test/unit-test/test-core-001.cpp
	test/unit-test/test-core-002.cpp
	test/unit-test/test-core-003.cpp
	test/unit-test/test-core-004.cpp
	test/unit-test/test-async-001.cpp
	test/unit-test/test-async-002.cpp
	test/unit-test/test-io-001.cpp
	test/unit-test/test-io-002.cpp
	test/unit-test/test-mail-001.cpp
	test/unit-test/test-log-001.cpp
	test/unit-test/test-store-001.cpp
	test/unit-test/test-store-002.cpp
	test/unit-test/test-vm-001.cpp
	test/unit-test/test-sql-001.cpp
	test/unit-test/test-parser-001.cpp
	test/unit-test/test-json-001.cpp
	test/unit-test/test-crypto-001.cpp
)
    
set (unit_test_h
    src/main/include/cyng/cyng.h
	test/unit-test/test-core-001.h
	test/unit-test/test-core-002.h
	test/unit-test/test-core-003.h
	test/unit-test/test-core-004.h
	test/unit-test/test-async-001.h
	test/unit-test/test-async-002.h
	test/unit-test/test-io-001.h
	test/unit-test/test-io-002.h
	test/unit-test/test-mail-001.h
	test/unit-test/test-log-001.h
	test/unit-test/test-store-001.h
	test/unit-test/test-store-002.h
	test/unit-test/test-vm-001.h
	test/unit-test/test-sql-001.h
	test/unit-test/test-parser-001.h
	test/unit-test/test-json-001.h
	test/unit-test/test-crypto-001.h
)

# define the unit test
set (unit_test
  ${unit_test_cpp}
  ${unit_test_h}
)
