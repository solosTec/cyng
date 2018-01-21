# top level files
set (db_lib)

set (db_cpp

    lib/db/src/session.cpp
    lib/db/src/session_pool.cpp
    lib/db/src/sql_table.cpp
    lib/db/src/session_wrapper.hpp
)

set (db_h

    src/main/include/cyng/db/connection_types.h
    src/main/include/cyng/db/interface_session.h
    src/main/include/cyng/db/interface_statement.h
    src/main/include/cyng/db/interface_result.h
    src/main/include/cyng/db/session.h
    src/main/include/cyng/db/session_pool.h

	src/main/include/cyng/table/meta_interface.h
    src/main/include/cyng/db/sql_table.h
    src/main/include/cyng/db/create_table.hpp
    src/main/include/cyng/db/delete_table.hpp
    src/main/include/cyng/db/drop_table.hpp
    
)


# define the database library
set (db_lib
  ${db_cpp}
  ${db_h}
)

#
# MySQL
#
if (CYNG_MYSQL_INSTALLED)
	set (db_mysql
		lib/sql/src/mysql/mydb_error.cpp
		lib/sql/src/mysql/mydb_error.hpp
		lib/sql/src/mysql/mydb_session.h
		lib/sql/src/mysql/mydb_session.cpp
		lib/sql/src/mysql/mydb_command.h
		lib/sql/src/mysql/mydb_command.cpp)

	list(APPEND db_lib ${db_mysql})
	source_group("mysql" FILES ${db_mysql})

endif (CYNG_MYSQL_INSTALLED)

#
# SQLite3
#
if (CYNG_SQLITE3_INSTALLED)
	set (db_sqlite3
		lib/db/src/sqlite/sqlite_defs.h
		lib/db/src/sqlite/sqlite_error.cpp
		lib/db/src/sqlite/sqlite_error.hpp
		lib/db/src/sqlite/sqlite_session.h
		lib/db/src/sqlite/sqlite_session.cpp
		lib/db/src/sqlite/sqlite_connection.h
		lib/db/src/sqlite/sqlite_connection.cpp
		lib/db/src/sqlite/sqlite_statement.h
		lib/db/src/sqlite/sqlite_statement.cpp
		lib/db/src/sqlite/sqlite_result.h
		lib/db/src/sqlite/sqlite_result.cpp)

	list(APPEND db_lib ${db_sqlite3})
	source_group("sqlite3" FILES ${db_sqlite3})

endif (CYNG_SQLITE3_INSTALLED)

#
# ODBC
#
if (CYNG_ODBC_INSTALLED)
	set (db_odbc
		lib/db/src/odbc/odbc_connection.cpp
		lib/db/src/odbc/odbc_connection.h
		lib/db/src/odbc/odbc_defs.h
		lib/db/src/odbc/odbc_diag.hpp
		lib/db/src/odbc/odbc_error.cpp
		lib/db/src/odbc/odbc_error.hpp
		lib/db/src/odbc/odbc_exception.hpp
		lib/db/src/odbc/odbc_handle.hpp
		lib/db/src/odbc/odbc_result.cpp
		lib/db/src/odbc/odbc_result.h
		lib/db/src/odbc/odbc_session.cpp
		lib/db/src/odbc/odbc_session.h
		lib/db/src/odbc/odbc_statement.h
		lib/db/src/odbc/odbc_statement.cpp)

	list(APPEND db_lib ${db_odbc})
	source_group("odbc" FILES ${db_odbc})

endif (CYNG_ODBC_INSTALLED)

