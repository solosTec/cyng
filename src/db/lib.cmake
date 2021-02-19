# 
#	reset 
#
set (db_lib)

set (db_cpp
    src/db/session.cpp
    src/db/connection_types.cpp
    src/db/session_pool.cpp
    src/db/storage.cpp
)
    
set (db_h
    include/cyng/db/session.h
    include/cyng/db/connection_types.h
    include/cyng/db/session_pool.h
    include/cyng/db/storage.h
)

set (db_details
    include/cyng/db/details/session_interface.h
    include/cyng/db/details/statement_interface.h
    include/cyng/db/details/result_interface.h
)

set (db_sqlite3
    src/db/sqlite3/sqlite_defs.h
    src/db/sqlite3/sqlite_session.h
    src/db/sqlite3/sqlite_session.cpp
    src/db/sqlite3/sqlite_connection.h
    src/db/sqlite3/sqlite_connection.cpp
    src/db/sqlite3/sqlite_error.hpp
    src/db/sqlite3/sqlite_error.cpp
    src/db/sqlite3/sqlite_result.h
    src/db/sqlite3/sqlite_result.cpp
    src/db/sqlite3/sqlite_statement.h
    src/db/sqlite3/sqlite_statement.cpp
)

set (db_odbc
    src/db/odbc/odbc_defs.h
    src/db/odbc/odbc_diag.hpp
    src/db/odbc/odbc_error.cpp
    src/db/odbc/odbc_error.hpp
    src/db/odbc/odbc_handle.hpp
    src/db/odbc/odbc_exception.hpp
    src/db/odbc/odbc_connection.h
    src/db/odbc/odbc_connection.cpp
    src/db/odbc/odbc_result.h
    src/db/odbc/odbc_result.cpp
    src/db/odbc/odbc_session.h
    src/db/odbc/odbc_session.cpp
    src/db/odbc/odbc_statement.h
    src/db/odbc/odbc_statement.cpp
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