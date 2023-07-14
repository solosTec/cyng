# 
#	reset 
#
set (db_lib)

set (db_cpp
    src/session.cpp
    src/connection_types.cpp
    src/session_pool.cpp
    src/storage.cpp
    src/julian.cpp
)
    
set (db_h
    ${CMAKE_SOURCE_DIR}/include/cyng/db/session.h
    ${CMAKE_SOURCE_DIR}/include/cyng/db/connection_types.h
    ${CMAKE_SOURCE_DIR}/include/cyng/db/session_pool.h
    ${CMAKE_SOURCE_DIR}/include/cyng/db/storage.h
    ${CMAKE_SOURCE_DIR}/include/cyng/db/julian.h
)

set (db_details
    ${CMAKE_SOURCE_DIR}/include/cyng/db/details/session_interface.h
    ${CMAKE_SOURCE_DIR}/include/cyng/db/details/statement_interface.h
    ${CMAKE_SOURCE_DIR}/include/cyng/db/details/result_interface.h
)

set (db_sqlite3
    src/sqlite3/sqlite_defs.h
    src/sqlite3/sqlite_session.h
    src/sqlite3/sqlite_session.cpp
    src/sqlite3/sqlite_connection.h
    src/sqlite3/sqlite_connection.cpp
    src/sqlite3/sqlite_error.hpp
    src/sqlite3/sqlite_error.cpp
    src/sqlite3/sqlite_result.h
    src/sqlite3/sqlite_result.cpp
    src/sqlite3/sqlite_statement.h
    src/sqlite3/sqlite_statement.cpp
)

set (db_odbc
    src/odbc/odbc_defs.h
    src/odbc/odbc_diag.hpp
    src/odbc/odbc_error.cpp
    src/odbc/odbc_error.hpp
    src/odbc/odbc_handle.hpp
    src/odbc/odbc_exception.hpp
    src/odbc/odbc_connection.h
    src/odbc/odbc_connection.cpp
    src/odbc/odbc_result.h
    src/odbc/odbc_result.cpp
    src/odbc/odbc_session.h
    src/odbc/odbc_session.cpp
    src/odbc/odbc_statement.h
    src/odbc/odbc_statement.cpp
)

set (db_duck
#    src/duckdb/duckdb_defs.h
    src/duckdb/duckdb_session.h
    src/duckdb/duckdb_session.cpp
#    src/duckdb/duckdb_connection.h
#    src/duckdb/duckdb_connection.cpp
#    src/duckdb/duckdb_error.hpp
#    src/duckdb/duckdb_error.cpp
#    src/duckdb/duckdb_result.h
#    src/duckdb/duckdb_result.cpp
#    src/duckdb/duckdb_statement.h
#    src/duckdb/duckdb_statement.cpp
)

source_group("details" FILES ${db_details})


# define the docscript lib
set (db_lib
  ${db_cpp}
  ${db_h}
  ${db_details}
)

if (${CAPITAL_NAME}_SQLITE3_CONNECTOR)
    source_group("SQLite3" FILES ${db_sqlite3})
    list(APPEND db_lib ${db_sqlite3})
endif()

if (${CAPITAL_NAME}_ODBC_CONNECTOR)
    source_group("ODBC" FILES ${db_odbc})
    list(APPEND db_lib ${db_odbc})
endif()

if (${CAPITAL_NAME}_DUCKDB_CONNECTOR)
    source_group("DuckDB" FILES ${db_duck})
    list(APPEND db_lib ${db_duck})
endif()