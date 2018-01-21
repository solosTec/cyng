
#
#
# CMake compatibility issues: don't modify this, please!
cmake_minimum_required (VERSION 3.5)

                                                     
#                                                                                                        
#                                                      
#                                                      
#            **   ****                                 
#    ****     **    ***  *  ***  ****        ****      
#   * ***  *  **     ****    **** **** *    *  ***  *  
#  *   ****   **      **      **   ****    *    ****   
# **          **      **      **    **    **     **    
# **          **      **      **    **    **     **    
# **          **      **      **    **    **     **    
# **          **      **      **    **    **     **    
# ***     *    *********      **    **    **     **    
#  *******       **** ***     ***   ***    ********    
#   *****              ***     ***   ***     *** ***   
#               *****   ***                       ***  
#             ********  **                  ****   *** 
#            *      ****                  *******  **  
#                                        *     ****    
#                                                      
# set project name
project(CYNG)

# Add a version number
set (${PROJECT_NAME}_VERSION_MAJOR 0)
set (${PROJECT_NAME}_VERSION_MINOR 2)

string(TIMESTAMP THIS_YEAR "%Y")
set (${PROJECT_NAME}_COPYRIGHT_YEAR ${THIS_YEAR})

# Patch level as year + day of the year
string(TIMESTAMP PATCH_LEVEL "%j")	# day of the year
math(EXPR ${PROJECT_NAME}_PATCH_LEVEL "(${THIS_YEAR} * 1000) + ${PATCH_LEVEL}")


message(STATUS "**   ")  
message(STATUS "**              **   ****                                  ")  
message(STATUS "**      ****     **    ***  *  ***  ****        ****       ")  
message(STATUS "**     * ***  *  **     ****    **** **** *    *  ***  *   ")  
message(STATUS "**    *   ****   **      **      **   ****    *    ****    ")  
message(STATUS "**   **          **      **      **    **    **     **     ")  
message(STATUS "**   **          **      **      **    **    **     **     ")  
message(STATUS "**   **          **      **      **    **    **     **     ")  
message(STATUS "**   **          **      **      **    **    **     **     ")  
message(STATUS "**   ***     *    *********      **    **    **     **     ")  
message(STATUS "**    *******       **** ***     ***   ***    ********     ")  
message(STATUS "**     *****              ***     ***   ***     *** ***    ")  
message(STATUS "**                 *****   ***                       ***   ")  
message(STATUS "**               ********  **                  ****   ***  ")  
message(STATUS "**              *      ****                  *******  **   ")  
message(STATUS "**                                          *     ****     ")  
message(STATUS "**   ")                                        


#
# Collect and dump some basic information
#
string(TIMESTAMP NOW_UTC "%Y-%m-%dT%H:%M:%SZ")
set (${PROJECT_NAME}_NOW_UTC ${NOW_UTC})

include(ProcessorCount)
ProcessorCount(${PROJECT_NAME}_CPU_COUNT)

message(STATUS "** CMake           : v${CMAKE_VERSION}")
message(STATUS "** Generator       : ${CMAKE_GENERATOR}")
message(STATUC "** Processor       : ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "** Platform        : ${CMAKE_SYSTEM}")
message(STATUS "** Compiler        : ${CMAKE_CXX_COMPILER_ID} v${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "** Timestamp       : ${${PROJECT_NAME}_NOW_UTC}")
message(STATUS "** CPU Cores       : ${${PROJECT_NAME}_CPU_COUNT}")

if(UNIX)
	# has no meaning on VS
	# set default cmake build type to RelWithDebInfo (None Debug Release RelWithDebInfo MinSizeRel)
	if( NOT CMAKE_BUILD_TYPE )
		set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
		message(STATUS "** Set build type  : ${CMAKE_BUILD_TYPE}")
	endif()
	message(STATUS "** Build type      : ${CMAKE_BUILD_TYPE}")
	find_program(LSB_RELEASE lsb_release)
	execute_process(COMMAND ${LSB_RELEASE} -is
		OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	message(STATUS "** System          : ${LSB_RELEASE_ID_SHORT}")
else()
	if( NOT CMAKE_BUILD_TYPE )
		set( CMAKE_BUILD_TYPE "Debug" )
		message(STATUS "** Set build type  : ${CMAKE_BUILD_TYPE}")
	endif()
	set( LSB_RELEASE_ID_SHORT ${CMAKE_SYSTEM_NAME} )

endif(UNIX)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(STATUS "** Address Model   : 64 bit")
	set(${PROJECT_NAME}_ADDRESS_MODEL 64)
#
#	Generate magic numbers
#
	string(RANDOM LENGTH 16 ALPHABET "1234567890ABCDEF" __RND_VALUE_NULL)
	string(RANDOM LENGTH 16 ALPHABET "1234567890ABCDEF" __RND_VALUE_EOD)
	string(RANDOM LENGTH 16 ALPHABET "1234567890ABCDEF" __RND_VALUE_DUMMY)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
	message(STATUS "** Address Model   : 32 bit")
	set(${PROJECT_NAME}_ADDRESS_MODEL 32)
#
#	Generate magic numbers
#
	string(RANDOM LENGTH 8 ALPHABET "1234567890ABCDEF" __RND_VALUE_NULL)
	string(RANDOM LENGTH 8 ALPHABET "1234567890ABCDEF" __RND_VALUE_EOD)
	string(RANDOM LENGTH 8 ALPHABET "1234567890ABCDEF" __RND_VALUE_DUMMY)
else()
	message(STATUS "** Address Model   : not supported")
	set(${PROJECT_NAME}_ADDRESS_MODEL 16)
#
#	Generate magic numbers
#
	string(RANDOM LENGTH 4 ALPHABET "1234567890ABCDEF" __RND_VALUE_NULL)
	string(RANDOM LENGTH 4 ALPHABET "1234567890ABCDEF" __RND_VALUE_EOD)
	string(RANDOM LENGTH 4 ALPHABET "1234567890ABCDEF" __RND_VALUE_DUMMY)
endif()

#
#	Generate magic numbers
#
set (${PROJECT_NAME}_NULL_HASH "0x${__RND_VALUE_NULL}")
set (${PROJECT_NAME}_EOD_HASH "0x${__RND_VALUE_EOD}")
set (${PROJECT_NAME}_DUMMY_HASH "0x${__RND_VALUE_DUMMY}")


#
#	setup C++ compiler
#
if (CMAKE_COMPILER_IS_GNUCC)

	#
	# gnu C++
	# see: https://stackoverflow.com/questions/36222912/is-c17-stdshared-mutex-not-available-yet
	#
	
	# -std=c++98
	# C++11 since 4.8.1
	# C++14 since 6.1
	# C++17 since 7.0 (?)
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "4.8.1")
		add_definitions(-std=c++98)	
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		message(STATUS "** C++ support     : C++98")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "6.1")
		add_definitions(-std=c++11)	
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		message(STATUS "** C++ support     : C++11")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "7.0")
		add_definitions(-std=c++14)	
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_OFF)
		message(STATUS "** C++ support     : C++14")
	else()
		add_definitions(-std=c++17)	
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_OFF)
		message(STATUS "** C++ support     : C++17")
	endif()


	# string_view since 7.3
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER  "7.2")
		set(${PROJECT_NAME}_STD_STRING_VIEW ${PROJECT_NAME}_STD_STRING_VIEW_ON)
    else()
		set(${PROJECT_NAME}_STD_STRING_VIEW ${PROJECT_NAME}_STD_STRING_VIEW_OFF)
	endif()
	
	# shared_mutex support since 6.1
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER  "6.0")
		set(${PROJECT_NAME}_STD_SHARED_MUTEX ${PROJECT_NAME}_STD_SHARED_MUTEX_ON)
    else()
		set(${PROJECT_NAME}_STD_SHARED_MUTEX ${PROJECT_NAME}_STD_SHARED_MUTEX_OFF)
	endif()
	
	# std::apply since 7.0
	# BOOST_NO_CXX17_STD_APPLY doesn't do the job
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "7.0")
		set(${PROJECT_NAME}_STD_APPLY ${PROJECT_NAME}_STD_APPLY_OFF)
		set(${PROJECT_NAME}_STD_UNIQUE_PTR ${PROJECT_NAME}_STD_UNIQUE_PTR_OFF)
    else()
		set(${PROJECT_NAME}_STD_APPLY ${PROJECT_NAME}_STD_APPLY_ON)
		set(${PROJECT_NAME}_STD_UNIQUE_PTR ${PROJECT_NAME}_STD_UNIQUE_PTR_ON)
	endif()
	
	set(GLOBAL_LIBRARY_TYPE SHARED)

elseif(MSVC)

	#
	# Microsoft C++
	#
	
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "18.0")
	# 	prior Visual Studio 2013
		message(FATAL_ERROR "Insufficient MSVC version")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "19.0")
	#	after Visual Studio 2015
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		set(${PROJECT_NAME}_STD_SHARED_MUTEX ${PROJECT_NAME}_STD_SHARED_MUTEX_ON)
		set(${PROJECT_NAME}_STD_STRING_VIEW	${PROJECT_NAME}_STD_STRING_VIEW_OFF)
		#	Even 19.5 lacks support for std::apply()
		set(${PROJECT_NAME}_STD_APPLY	${PROJECT_NAME}_STD_APPLY_OFF)
		set(${PROJECT_NAME}_STD_UNIQUE_PTR ${PROJECT_NAME}_STD_UNIQUE_PTR_ON)
	else()
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		set(${PROJECT_NAME}_STD_SHARED_MUTEX ${PROJECT_NAME}_STD_SHARED_MUTEX_OFF)
		set(${PROJECT_NAME}_STD_STRING_VIEW	${PROJECT_NAME}_STD_STD_STRING_VIEW_OFF)
		set(${PROJECT_NAME}_STD_APPLY	${PROJECT_NAME}_STD_APPLY_OFF)
		set(${PROJECT_NAME}_STD_UNIQUE_PTR ${PROJECT_NAME}_STD_UNIQUE_PTR_OFF)
	endif()
	
	set(GLOBAL_LIBRARY_TYPE STATIC)

	#	This is Windows 7 (and rules out windows vista)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0601 -D_SCL_SECURE_NO_WARNINGS -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS")	
	add_definitions(/EHsc /bigobj /std:c++latest /MP)
	
	#
	# help CMake to find Boost
	#
	if(NOT BOOST_ROOT)
		set(BOOST_ROOT "C:/local/boost_1_66_0/" CACHE PATH "BOOST_ROOT" )
		message(STATUS "** Set BOOST_ROOT  : ${BOOST_ROOT}")
	endif()

	#
	# help CMake to find OpenSSL
	#
	if(NOT OPENSSL_ROOT_DIR)
		set(OPENSSL_ROOT_DIR "C:/local/OpenSSL-Win64" CACHE PATH "OPENSSL_ROOT_DIR")
		message(STATUS "** Set OPENSSL_ROOT_DIR: ${OPENSSL_ROOT_DIR}")
	endif()
	
else()

	message( FATAL_ERROR "Unknown or missing compiler: ${CMAKE_CXX_COMPILER_ID}" )
	
endif()

#
#	setup Boost library 1.66.0
#
# if(UNIX)
# set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_66_0/release")
# set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_66_0/release/lib")
# endif(UNIX)
set(Boost_ADDITIONAL_VERSIONS "1.61.0" "1.62.0" "1.63.0" "1.64.0" "1.65.1" "1.66.0")
message(STATUS "** Search Boost    : ${Boost_ADDITIONAL_VERSIONS}")
find_package(Boost 1.61 REQUIRED COMPONENTS thread system date_time filesystem program_options random signals unit_test_framework regex)
#find_package(Boost 1.61 REQUIRED COMPONENTS thread system date_time filesystem)

if(Boost_FOUND)

	message(STATUS "** Boost Version    : ${Boost_VERSION}")
	message(STATUS "** Boost Include    : ${Boost_INCLUDE_DIRS}")
	message(STATUS "** Boost Path       : ${Boost_LIBRARY_DIRS}")
	message(STATUS "** Boost Libraries  : ${Boost_LIBRARIES}")

if(UNIX)
	include_directories($ENV{HOME}/projects/beast/include)
	include_directories($ENV{HOME}/projects/asio/include)
endif()

	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
	
	set(${PROJECT_NAME}_BOOST_LIBRARY ${PROJECT_NAME}_BOOST_LIBRARY_FOUND)
	set(${PROJECT_NAME}_BOOST_VERSION ${PROJECT_NAME}_BOOST_${Boost_VERSION})
	
	# support for unicode
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_SPIRIT_UNICODE")
	
else()
	set(${PROJECT_NAME}_BOOST_LIBRARY ${PROJECT_NAME}_BOOST_LIBRARY_NOT_FOUND)
endif(Boost_FOUND)

#
#	setup SSL library
#
message(STATUS "** Search openSSL   : ${OPENSSL_ROOT_DIR}")
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
	find_package(OpenSSL 1.0.2 REQUIRED)
else()
	find_package(OpenSSL 1.0.1 REQUIRED)
endif()
if(OPENSSL_FOUND)

#	message(STATUS "** openSSL Found         : ${OPENSSL_FOUND}")
	message(STATUS "** openSSL Version       : ${OPENSSL_VERSION}")
	message(STATUS "** openSSL Include       : ${OPENSSL_INCLUDE_DIR}")
	message(STATUS "** openSSL crypto library: ${OPENSSL_CRYPTO_LIBRARY}")
	message(STATUS "** openSSL SSL library   : ${OPENSSL_SSL_LIBRARY}")
	message(STATUS "** openSSL Libraries     : ${OPENSSL_LIBRARIES}")
 
 	include_directories(${OPENSSL_INCLUDE_DIR})
 	link_directories(${OPENSSL_LIBRARIES})
	
endif()

#
#	The path to cmake modules
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/modules/")

#
#	Find SQLite3/UnixODBC library
#
if (UNIX)

	#
	#	SQLite3
	#
	find_package(SQLite3)
	if(SQLite3_FOUND)

		include_directories(${SQLite3_INCLUDE_DIR})
		link_directories(${SQLITE3_LIBRARY})
  
		message(STATUS "** SQLite3 Version   : ${SQLite3_VERSION}")
		message(STATUS "** SQLite3 Include   : ${SQLite3_INCLUDE_DIR}")
		message(STATUS "** SQLite3 Libraries : ${SQLite3_LIBRARY}")
	
		#
		#	define this compile flag if SQLite3 library is available
		#
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D${PROJECT_NAME}_SQLITE3_INSTALLED" )
		set(${PROJECT_NAME}_SQLITE3_INSTALLED ON)

	else()

		message(WARNING "** SQLite3 not found")
#		set(SQLite3_LIBRARY "")
		set(${PROJECT_NAME}_SQLITE3_INSTALLED OFF)

	endif()

	#
	#	Find unixODBC library
	#
	find_package(UnixODBC)
	if(UnixODBC_FOUND)

		include_directories(${UnixODBC_INCLUDE_DIRS})
		link_directories(${UnixODBC_LIBRARIES})
	
		message(STATUS "** UnixODBC Include  : ${UnixODBC_INCLUDE_DIRS}")
		message(STATUS "** UnixODBC Libraries: ${UnixODBC_LIBRARIES}")
		
		#
		#	define this compile flag if ODBC C++ is available
		#
		set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D${PROJECT_NAME}_ODBC_INSTALLED" )
		set(${PROJECT_NAME}_ODBC_INSTALLED ON)

	else()

		message(WARNING "** UnixODBC not found")
#		set(UnixODBC_LIBRARIES "")
		set(${PROJECT_NAME}_ODBC_INSTALLED OFF)

	endif()	

elseif(WIN32)

	#
	#	SQLite3 library is part of this project
	#
	include (3party/sqlite.cmake)
	add_library(lib_sqlite3 ${LIBRARY_TYPE} ${sqlite_lib})

	#
	#
	#	SQLite3 shell
	include (3party/shell.cmake)
	add_executable(sqlite3 ${shell})
	target_link_libraries(sqlite3 lib_sqlite3)

	#
	#
	#	Link this library
	set(${PROJECT_NAME}_SQLITE3_INSTALLED ON)
	set(SQLite3_LIBRARY ${lib_sqlite3})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${PROJECT_NAME}_SQLITE3_INSTALLED" )

	#
	#	ODBC is always available on windows
	#
	set(${PROJECT_NAME}_ODBC_INSTALLED ON)
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D${PROJECT_NAME}_ODBC_INSTALLED" )

	message(STATUS "** SQlite3 Version       : ${SQLite3_VERSION}")

else()
	message(WARNING "** platform not supported")
endif()

set(${PROJECT_NAME}_MYSQL_INSTALLED OFF)
set(${PROJECT_NAME}_OLEDB_INSTALLED OFF)


#
# include directories for all C/C++ projects
#
include_directories(src/main/include)

#
# configure a header file to pass some of the CMake settings
# to the source code
#
configure_file (
  "${PROJECT_SOURCE_DIR}/src/main/templates/project_info.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_project_info.h"
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#
#	cyng meta library - header only
#
include (lib/meta/lib.cmake)
add_library(cyng_meta ${GLOBAL_LIBRARY_TYPE} ${meta_lib})


#
#	cyng core/object library
#
include (lib/core/lib.cmake)
add_library(cyng_core ${GLOBAL_LIBRARY_TYPE} ${core_lib})
if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
	target_link_libraries(cyng_core boost_thread)
endif()

#
#	cyng log library
#
include (lib/log/lib.cmake)
add_library(cyng_log ${GLOBAL_LIBRARY_TYPE} ${log_lib})

#
#	cyng I/O library
#
include (lib/io/lib.cmake)
add_library(cyng_io ${GLOBAL_LIBRARY_TYPE} ${io_lib})
target_link_libraries(cyng_io
	cyng_core)

#
#	cyng mail library
#
include (lib/io/mail/lib.cmake)
add_library(cyng_mail ${GLOBAL_LIBRARY_TYPE} ${mail_lib})

#
#	cyng VM library
#
include (lib/vm/lib.cmake)
add_library(cyng_vm ${GLOBAL_LIBRARY_TYPE} ${vm_lib})
target_link_libraries(cyng_vm
	cyng_core
	cyng_io)

#
#	cyng domain library
#
include (lib/domain/lib.cmake)
add_library(cyng_domain ${GLOBAL_LIBRARY_TYPE} ${domain_lib})
target_link_libraries(cyng_domain
	cyng_store
	cyng_vm)

#
#	cyng async library
#
include (lib/async/lib.cmake)
add_library(cyng_async ${GLOBAL_LIBRARY_TYPE} ${async_lib})

#
#	cyng store library
#
include (lib/store/lib.cmake)
add_library(cyng_store ${GLOBAL_LIBRARY_TYPE} ${store_lib})
target_link_libraries(cyng_store
	cyng_core)

#
#	cyng table library
#
include (lib/table/lib.cmake)
add_library(cyng_table ${GLOBAL_LIBRARY_TYPE} ${table_lib})

#
#	cyng sql library
#
include (lib/sql/lib.cmake)
add_library(cyng_sql ${GLOBAL_LIBRARY_TYPE} ${sql_lib})
target_link_libraries(cyng_sql
	cyng_store)

#
#	cyng crypto library
#
include (lib/crypto/lib.cmake)
add_library(cyng_crypto ${GLOBAL_LIBRARY_TYPE} ${crypto_lib})
target_link_libraries(cyng_crypto
	${OPENSSL_LIBRARIES})

#
#	cyng parser library
#
include (lib/parser/lib.cmake)
add_library(cyng_parser ${GLOBAL_LIBRARY_TYPE} ${parser_lib})
target_link_libraries(cyng_parser
	cyng_core)

#
#	cyng JSON library
#
include (lib/json/lib.cmake)
add_library(cyng_json ${GLOBAL_LIBRARY_TYPE} ${json_lib})
target_link_libraries(cyng_json
	cyng_core
	cyng_io
	cyng_parser)

#
#	cyng docScript library
#
include (lib/docscript/lib.cmake)
add_library(cyng_docscript ${GLOBAL_LIBRARY_TYPE} ${docscript_lib})
target_link_libraries(cyng_docscript
	cyng_core
	cyng_async
	cyng_crypto
	cyng_vm)

#
#	cyng system library
#
include (lib/sys/lib.cmake)
add_library(cyng_sys ${GLOBAL_LIBRARY_TYPE} ${sys_lib})

#
#	cyng db library
#
include (lib/db/lib.cmake)
add_library(cyng_db ${GLOBAL_LIBRARY_TYPE} ${db_lib})
set(db_link_libs )
if (${PROJECT_NAME}_ODBC_INSTALLED)
	if(WIN32)
		list(APPEND db_link_libs odbc32.lib)
	else()
		list(APPEND db_link_libs ${UnixODBC_LIBRARIES})
	endif(WIN32)
endif()
if (${PROJECT_NAME}_SQLITE3_INSTALLED)
	if(WIN32)
		list(APPEND db_link_libs lib_sqlite3)
else()
		list(APPEND db_link_libs ${SQLite3_LIBRARY})
	endif(WIN32)
endif()
target_link_libraries(cyng_db ${db_link_libs})
#message(STATUS "** db link library : ${db_link_libs}")


#
# minimal test program
# hello world!
# cross compile with 
# arm-linux-gnueabihf-g++ -O3 -g3 -Wall -fPIC -o "main.o" -c "main.cpp"
# arm-linux-gnueabihf-g++ -o "hello" main.o
#
include (test/hello/hello.cmake)
add_executable(hello ${hello})

#
# demo/example code
#
include (test/demo.cmake)
add_executable(demo ${demo})
# libraries to link
target_link_libraries(demo
# 	cyng_core
	${Boost_LIBRARIES})
	
#
# test unit using Boost.Test
# BOOST_TEST_DYN_LINK is required to build a main() function
#
include (test/unit-test/unit-test.cmake)
add_executable(unit_test ${unit_test})
	
# BOOST_TEST_DYN_LINK is required to build a main() function
set_property(
	TARGET unit_test
	PROPERTY COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK BOOST_ASIO_HAS_MOVE)

set(unittest_link_libs cyng_core cyng_io cyng_async cyng_log cyng_store cyng_vm cyng_domain cyng_sql cyng_json cyng_parser cyng_mail cyng_crypto cyng_sys cyng_db cyng_table)

if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
	list(APPEND unittest_link_libs "${Boost_LIBRARIES} ${OPENSSL_LIBRARIES}")
endif()

target_link_libraries(unit_test ${unittest_link_libs})

#
# docScript compiler
#
include (tools/docc/prg.cmake)
add_executable(docc ${docc})
set(docc_link_libs cyng_io cyng_vm cyng_async cyng_docscript)
if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
	list(APPEND docc_link_libs "${Boost_LIBRARIES}")
endif()
target_link_libraries(docc ${docc_link_libs})
