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
include (${CMAKE_SOURCE_DIR}/3party/sqlite.cmake)
add_library(cyng_sqlite3 ${GLOBAL_LIBRARY_TYPE} ${sqlite_lib})
add_library(cyng::sqlite3 ALIAS "cyng_sqlite3")

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

if(NOT ${PROJECT_NAME}_CROSS_COMPILE)
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
include (${CMAKE_SOURCE_DIR}/3party/shell.cmake)
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

endif()	

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
