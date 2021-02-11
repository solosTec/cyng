# 
#	reset 
#
set (db_lib)

set (db_cpp
    src/db/session.cpp
    src/db/connection_types.cpp
)
    
set (db_h
    include/cyng/db/session.h
    include/cyng/db/connection_types.h
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
)

source_group("details" FILES ${db_details})
source_group("SQLite3" FILES ${db_sqlite3})
source_group("ODBC" FILES ${db_odbc})


# define the docscript lib
set (db_lib
  ${db_cpp}
  ${db_h}
  ${db_details}
  ${db_sqlite3}
  ${db_odbc}
)

