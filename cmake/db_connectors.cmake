#
#	detect available database connectors
#

#
# SQLite3 library
#
#   Don't use a possibly installed SQLite3 library. Instead use
#   the version shipped with the repository 3rd library path.
#   SQLite3 library is part of this project
#
include (${PROJECT_SOURCE_DIR}/3party/sqlite.cmake)
add_library(cyng_sqlite3 ${GLOBAL_LIBRARY_TYPE} ${sqlite_lib})
#add_library(cyng::sqlite3 ALIAS "cyng_sqlite3")

target_link_libraries(cyng_sqlite3 
    PRIVATE
        "$<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS};pthread>"
)

set_property(TARGET cyng_sqlite3 PROPERTY POSITION_INDEPENDENT_CODE ON)
set(${CAPITAL_NAME}_SQLITE3_CONNECTOR ON CACHE BOOL "SQLite3 Connector")

# data comes from sqlite.cmake
message(STATUS "** SQLite3 Version    : ${SQLite3_VERSION}")
message(STATUS "** SQLite3 Include    : ${SQLite3_INCLUDE_DIR}")
message(STATUS "** SQLite3 Libraries  : ${SQLite3_LIBRARY}")

if(NOT ${PROJECT_NAME}_CROSS_COMPILE AND NOT WIN32)
	#
	#	SQLite3 shell works better with readline support,
	# 	but to tricky on OECP platform
	#
	find_package(Readline)

	if (${READLINE_FOUND})
		message(STATUS "** readline include      : ${Readline_INCLUDE_DIR}")
		message(STATUS "** readline library      : ${Readline_LIBRARY}")
	endif()
endif()

#
#
#	SQLite3 shell
include (${PROJECT_SOURCE_DIR}/3party/shell.cmake)
add_executable(sqlite3 ${shell})

set_property(TARGET sqlite3 PROPERTY LINKER_LANGUAGE C)

target_link_libraries(sqlite3 
	PRIVATE
		cyng_sqlite3 
		"$<$<BOOL:${READLINE_FOUND}>:readline>"
		"$<$<PLATFORM_ID:Linux>:pthread;${CMAKE_DL_LIBS}>"
)


#   http://pqxx.org/development/libpqxx/
#
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)
if (PQXX_LIB_FOUND)
#
#   add libpqxx to the available database connectors
#
    message(STATUS "** PostgreSQL  : <LANG>_VISIBILITY_PRESET available")
	set(${CAPITAL_NAME}_PQXX_CONNECTOR ON CACHE BOOL "PostgreSQL Connector")

else()
    message(WARNING "libpqxx (PostgreSQL) not found")
	set(${CAPITAL_NAME}_PQXX_CONNECTOR OFF CACHE BOOL "PostgreSQL Connector")
endif()

#
#	FindODBC
#	ODBC is always available on windows
#
find_package(ODBC)
if (ODBC_FOUND)
	
	message(STATUS "** ODBC Include      : ${ODBC_INCLUDE_DIRS}")
	message(STATUS "** ODBC Libraries    : ${ODBC_LIBRARIES}")
	message(STATUS "** ODBC Config       : ${ODBC_CONFIG}")
		
	#
	#	define this compile flag if ODBC C++ is available
	#
	set(${CAPITAL_NAME}_ODBC_CONNECTOR ON CACHE BOOL "ODBC Connector")

else()

	message(WARNING "** ODBC not found")
	set(${CAPITAL_NAME}_ODBC_CONNECTOR OFF CACHE BOOL "ODBC Connector")

endif(ODBC_FOUND)	

#
#	MySQL: https://github.com/mysql/mysql-connector-odbc/blob/master/cmake/FindMySQL.cmake
#
#find_package(MySQL)
#if(MYSQL_FOUND)
#    message(STATUS "MySQL ${MYSQL_INCLUDE_DIR}")
#	set(${PROJECT_NAME}_MYSQL_CONNECTOR ON CACHE BOOL "MySQL Connector")
#
#endif()
set(${CAPITAL_NAME}_MYSQL_CONNECTOR OFF CACHE BOOL "MySQL Connector")


#
# https://github.com/duckdb/duckdb
# https://github.com/duckdb/duckdb/releases/download/v0.8.0/libduckdb-src.zip
#
 set(CYNG_DUCKDB_CONNECTOR OFF CACHE BOOL "Add DuckDB sources")
 if (CYNG_DUCKDB_CONNECTOR)

#	set(DUCKDB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/3party/libduckdb-src")

#	message(STATUS "** DuckDB Include    : ${DUCKDB_INCLUDE_DIRS}")

	include (${PROJECT_SOURCE_DIR}/3party/duckdb.cmake)
#	add_library(cyng_duckdb ${GLOBAL_LIBRARY_TYPE} ${duckdb_lib})
	add_library(cyng_duckdb SHARED ${duckdb_lib})
	add_library(cyng::duckdb ALIAS "cyng_duckdb")
	set_property(TARGET cyng_duckdb PROPERTY POSITION_INDEPENDENT_CODE ON)
	target_compile_definitions(cyng_duckdb
		PRIVATE 
			"DUCKDB_BUILD_LIBRARY"
	)
	target_link_libraries(cyng_duckdb
		PRIVATE
			"$<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS};pthread>"
	)

	add_library(cyng_duckdb_static STATIC ${duckdb_lib})
	add_library(cyng::duckdb_static ALIAS "cyng_duckdb_static")
	target_compile_definitions(cyng_duckdb_static
		PRIVATE 
			"DUCKDB_BUILD_LIBRARY"
	)
	target_link_libraries(cyng_duckdb_static
		PRIVATE
			"$<$<PLATFORM_ID:Linux>:${CMAKE_DL_LIBS};pthread>"
			"$<$<CXX_COMPILER_ID:MSVC>:ws2_32>"
	)

	# data comes from sqlite.cmake
	message(STATUS "** DuckDB  Version    : ${duckdb_VERSION}")
	message(STATUS "** DuckDB  Include    : ${duckdb_INCLUDE_DIR}")
	message(STATUS "** DuckDB  Libraries  : ${duckdb_LIBRARY}")

 
endif(CYNG_DUCKDB_CONNECTOR)	
