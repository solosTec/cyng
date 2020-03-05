#
#
# CMake compatibility issues: don't modify this, please!
cmake_minimum_required (VERSION 3.10)

                                                     
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
# get timestamp and build a patch level from the year and
# the day of the year. This is a unique number to distinguish
# different builds
string(TIMESTAMP THIS_YEAR "%Y")
# Patch level as year + day of the year
string(TIMESTAMP PATCH_LEVEL "%j")	# day of the year
math(EXPR PATCH_LEVEL "(${THIS_YEAR} * 1000) + ${PATCH_LEVEL}")

#
# set project name/properties
#
project(CYNG 
	VERSION 0.8.${PATCH_LEVEL}.1
	LANGUAGES CXX C
	DESCRIPTION "dynamic datatypes in C++")
#	HOMEPAGE_URL "https://solostec.ch")

string(TIMESTAMP THIS_YEAR "%Y")
set (${PROJECT_NAME}_COPYRIGHT_YEAR ${THIS_YEAR})


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
message(STATUS "** System          : ${CMAKE_SYSTEM_NAME}")
message(STATUS "** Compiler        : ${CMAKE_CXX_COMPILER_ID} v${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "** Timestamp       : ${${PROJECT_NAME}_NOW_UTC}")
message(STATUS "** CPU Cores       : ${${PROJECT_NAME}_CPU_COUNT}")

if(UNIX)
	# set default cmake build type to RelWithDebInfo (None Debug Release RelWithDebInfo MinSizeRel)
	if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Build Type")
		message(STATUS "** Set build type  : ${CMAKE_BUILD_TYPE}")
	endif()
	message(STATUS "** Build type      : ${CMAKE_BUILD_TYPE}")
	
	# get OS info
	find_program(LSB_RELEASE lsb_release)
	execute_process(COMMAND ${LSB_RELEASE} -is
		OUTPUT_VARIABLE LSB_RELEASE_ID_SHORT
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	message(STATUS "** System          : ${LSB_RELEASE_ID_SHORT}")
else()
	# has no meaning on VS
	if( NOT CMAKE_BUILD_TYPE )
		set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build Type")
		message(STATUS "** Set build type  : ${CMAKE_BUILD_TYPE}")
	endif()
	
	# no lsb_release available
	set(LSB_RELEASE_ID_SHORT ${CMAKE_SYSTEM_NAME} )

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
# manage unit test: CYNG_BUILD_TEST
# default is OFF
#
if(NOT DEFINED ${PROJECT_NAME}_BUILD_TEST)
	set(${PROJECT_NAME}_BUILD_TEST OFF CACHE BOOL "build unit test")
endif()

#
#	The path to cmake modules
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/modules/")

#
#	init submodules
#
include(InitSubmodules)

#
#	setup compiler features
#
include(CPPFeatureSupport)

#
#	setup C++ compiler
#
if (CMAKE_COMPILER_IS_GNUCXX)

	#
	# gnu C++
	# see: https://stackoverflow.com/questions/36222912/is-c17-stdshared-mutex-not-available-yet
	#
	
	#
	# gnu C++ select dialect
	#
	
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "4.7")
		set(CMAKE_CXX_STANDARD 11 CACHE STRING "C++ standard to be used")
		message(STATUS "** C++ support     : C++11")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "5.1")
	# default since  GCC 6.1
		set(CMAKE_CXX_STANDARD 14 CACHE STRING "C++ standard to be used")
		message(STATUS "** C++ support     : C++14")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "8.1")
    # partly since 5.0
		set(CMAKE_CXX_STANDARD 17 CACHE STRING "C++ standard to be used")
		message(STATUS "** C++ support     : C++17")
	else()
    # partly since 8.0
		if(${CMAKE_VERSION} VERSION_LESS "3.12.4") 
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")			
			message(STATUS "** C++ support     : C++2a")
		else()
			set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard to be used")
			message(STATUS "** C++ support     : C++2a")
		endif()
		
		#	suppress messages about ABI incompatibilities
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-psabi")			
	endif()
		
	set(GLOBAL_LIBRARY_TYPE SHARED)

elseif(MSVC)

	#
	# Microsoft C++
	#
	
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "18.0")
	# 	prior Visual Studio 2013
		message(FATAL_ERROR "Insufficient MSVC version")
	endif()
	
	set(GLOBAL_LIBRARY_TYPE STATIC)

	#	This is Windows 7 (and rules out windows vista)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /bigobj /std:c++latest /MP")	

	#
	#	Target is Windows 10
	#
	add_compile_definitions(_WIN32_WINNT=0x0A00 _SCL_SECURE_NO_WARNINGS _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
	
else()

	message( FATAL_ERROR "Unknown or missing compiler: ${CMAKE_CXX_COMPILER_ID}" )
	
endif()

#
#	add NODE_ADDRESS_MODEL=N
#
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.12.4") 
	add_compile_definitions(${PROJECT_NAME}_ADDRESS_MODEL=${${PROJECT_NAME}_ADDRESS_MODEL})
endif()

#
#	setup Boost library
#
include(SelectBoostLibrary)

#
#   Don't use a possibly installed SQLite3 library. Instead use
#   version shipped with the repository 3rd library path.
#   SQLite3 library is part of this project
include (3party/sqlite.cmake)
add_library(cyng_sqlite3 ${LIBRARY_TYPE} ${sqlite_lib})
set_property(TARGET cyng_sqlite3 PROPERTY POSITION_INDEPENDENT_CODE ON)
add_definitions(-D${PROJECT_NAME}_SQLITE3_INSTALLED)
set(${PROJECT_NAME}_SQLITE3_INSTALLED ON CACHE BOOL "SQLite3 Connector")

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
include (3party/shell.cmake)
add_executable(sqlite3 ${shell})

set_property(TARGET sqlite3 PROPERTY LINKER_LANGUAGE C)

target_link_libraries(sqlite3 
	PRIVATE
		cyng_sqlite3 
		"$<$<BOOL:${READLINE_FOUND}>:readline>"
		"$<$<PLATFORM_ID:Linux>:pthread;${CMAKE_DL_LIBS}>"
)

#
#
#	Link this library
set(SQLite3_LIBRARY ${lib_sqlite3})

# data comes from sqlite.cmake
message(STATUS "** SQLite3 Version   : ${SQLite3_VERSION}")
message(STATUS "** SQLite3 Include   : ${SQLite3_INCLUDE_DIR}")
message(STATUS "** SQLite3 Libraries : ${SQLite3_LIBRARY}")


#
#	Find UnixODBC library
#
if (UNIX)

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
		add_definitions(-D${PROJECT_NAME}_ODBC_INSTALLED)
		set(${PROJECT_NAME}_ODBC_INSTALLED ON CACHE BOOL "ODBC Connector")

	else()

		message(WARNING "** UnixODBC not found")
		set(${PROJECT_NAME}_ODBC_INSTALLED OFF CACHE BOOL "ODBC Connector")

	endif()	

elseif(WIN32)

	#
	#	ODBC is always available on windows
	#
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D${PROJECT_NAME}_ODBC_INSTALLED" )
	set(${PROJECT_NAME}_ODBC_INSTALLED ON CACHE BOOL "ODBC Connector")

else()
	message(WARNING "** platform not supported")
endif()

if (UNIX)
#
#   how to use MySQL Connector on linux:
#   sudo apt-get install libmysqlcppconn7v5
#
# https://dev.mysql.com/doc/connector-cpp/8.0/en/connector-cpp-source-configuration-options.html#option_cmake_cmake_install_prefix
# git clone https://github.com/mysql/mysql-connector-cpp
# cd mysql-connector-cpp/
# mkdir build && cd build
# cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local -DWITH_CONCPP=/usr/local/lib -DBOOST_ROOT=$HOME/projects/boost_1_68_0/install
# sudo cmake --build . --target install
#
# includes: /usr/local/mysql/connector-c++-8.0/include/
# lib:
#
#   try: cmake -DMYSQL_CONNECTOR_ROOT_DIR:PATH=../mysql-connector-cpp/build ..
#
    if(NOT MYSQL_CONNECTOR_ROOT_DIR)
        set(${PROJECT_NAME}_MYSQL_INSTALLED OFF CACHE BOOL "MySQL Connector")
        message(WARNING "** MySQL Connector not available")
    else()
        set(${PROJECT_NAME}_MYSQL_INSTALLED ON CACHE BOOL "MySQL Connector")
        set(${PROJECT_NAME}_MYSQL_ROOT ${MYSQL_CONNECTOR_ROOT_DIR} CACHE PATH "MYSQL_CONNECTOR_ROOT_DIR")
        message(STATUS "** MySQL Connector available at ${${PROJECT_NAME}_MYSQL_ROOT}")

        set(${PROJECT_NAME}_MYSQL_INCLUDE "${MYSQL_CONNECTOR_ROOT_DIR}/include")
        set(${PROJECT_NAME}_MYSQL_LIB "${MYSQL_CONNECTOR_ROOT_DIR}/build")
        include_directories(${PROJECT_NAME}_MYSQL_INCLUDE)
        link_directories(${PROJECT_NAME}_MYSQL_LIB)
    endif()
else()
    set(${PROJECT_NAME}_MYSQL_INSTALLED OFF CACHE BOOL "MySQL Connector")
    message(STATUS "** MySQL Connector not available")
endif()


#
#   OLEDB is not supported yet
#
set(${PROJECT_NAME}_OLEDB_INSTALLED OFF CACHE BOOL "OLEDB Connector")

#
# pugixml-master (v1.9)
# On Windows set CMake variables PugiXML_INCLUDE_DIRS and PugiXML_LIBRARIES 
# Included as git submodule with
# git submodule update --init --recursive
#
add_subdirectory(lib/xml/pugixml)
set(PUGIXML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/lib/xml/pugixml/src")
message(STATUS "** PugiXML Include   : ${PUGIXML_INCLUDE_DIR}")
add_definitions(-D${PROJECT_NAME}_PUGIXML_INSTALLED)

#
# configure a header file to pass some of the CMake settings
# to the source code
#
configure_file (
  "${PROJECT_SOURCE_DIR}/src/main/templates/project_info.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}_project_info.h"
)

#
# configure a pkgconfig file 
#
if(UNIX)
	configure_file (
	  "${PROJECT_SOURCE_DIR}/src/main/templates/pkgconfig/libcyng.pc.in"
	  "${CMAKE_CURRENT_BINARY_DIR}/libcyng.pc"
	  @ONLY)
endif(UNIX)

#
#	cyng meta library - header only
#
include (lib/meta/lib.cmake)
add_library(cyng_meta ${GLOBAL_LIBRARY_TYPE} ${meta_lib})
install(FILES 
        ${meta_lib}
    DESTINATION 
        include
    COMPONENT
        Devel
    )

#
#	cyng core/object library
#
include (lib/core/lib.cmake)
add_library(cyng_core ${GLOBAL_LIBRARY_TYPE} ${core_lib})
target_include_directories(cyng_core
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng log library
#
include (lib/log/lib.cmake)
add_library(cyng_log ${GLOBAL_LIBRARY_TYPE} ${log_lib})
target_include_directories(cyng_log
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng I/O library
#
include (lib/io/lib.cmake)
add_library(cyng_io ${GLOBAL_LIBRARY_TYPE} ${io_lib})
target_include_directories(cyng_io
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng VM library
#
include (lib/vm/lib.cmake)
add_library(cyng_vm ${GLOBAL_LIBRARY_TYPE} ${vm_lib})
target_include_directories(cyng_vm
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng async library
#
include (lib/async/lib.cmake)
add_library(cyng_async ${GLOBAL_LIBRARY_TYPE} ${async_lib})
target_include_directories(cyng_async
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng table library
#
include (lib/table/lib.cmake)
add_library(cyng_table ${GLOBAL_LIBRARY_TYPE} ${table_lib})
target_include_directories(cyng_table
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng store library
#
include (lib/store/lib.cmake)
add_library(cyng_store ${GLOBAL_LIBRARY_TYPE} ${store_lib})
target_include_directories(cyng_store
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng domain library
#
include (lib/domain/lib.cmake)
add_library(cyng_domain ${GLOBAL_LIBRARY_TYPE} ${domain_lib})
target_include_directories(cyng_domain
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng sql library
#
include (lib/sql/lib.cmake)
add_library(cyng_sql ${GLOBAL_LIBRARY_TYPE} ${sql_lib})
target_include_directories(cyng_sql
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng random library
#
include (lib/rnd/lib.cmake)
add_library(cyng_rnd ${GLOBAL_LIBRARY_TYPE} ${rnd_lib})
target_include_directories(cyng_rnd
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng parser library
#
include (lib/parser/lib.cmake)
add_library(cyng_parser ${GLOBAL_LIBRARY_TYPE} ${parser_lib})
target_include_directories(cyng_parser
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng JSON library
#
include (lib/json/lib.cmake)
add_library(cyng_json ${GLOBAL_LIBRARY_TYPE} ${json_lib})
target_include_directories(cyng_json
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)
# linker needs a hint of dependencies
target_link_libraries(cyng_json cyng_parser)

#
#	cyng CSV library
#
include (lib/csv/lib.cmake)
add_library(cyng_csv ${GLOBAL_LIBRARY_TYPE} ${csv_lib})
target_include_directories(cyng_csv
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)

#
#	cyng XML library
#
include (lib/xml/lib.cmake)
add_library(cyng_xml ${GLOBAL_LIBRARY_TYPE} ${xml_lib})
target_include_directories(cyng_xml
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${PUGIXML_INCLUDE_DIR}
		${BOOST_INCLUDE_DIR}
)

#
#	cyng system library
#
include (lib/sys/lib.cmake)
add_library(cyng_sys ${GLOBAL_LIBRARY_TYPE} ${sys_lib})
target_include_directories(cyng_sys
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)
# on windows:	-DPSAPI_VERSION=1
target_compile_definitions(cyng_sys 
    PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:"PSAPI_VERSION=1">
)

#
#	cyng db library
#
include (lib/db/lib.cmake)
add_library(cyng_db ${GLOBAL_LIBRARY_TYPE} ${db_lib})
target_include_directories(cyng_db
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
		${BOOST_INCLUDE_DIR}
)
set(db_link_libs cyng_sqlite3)
if(UNIX)
    list(APPEND db_link_libs ${CMAKE_DL_LIBS} cyng_parser)

    if (${PROJECT_NAME}_MYSQL_INSTALLED)
        #    libmysqlcppconn8.so
        message(STATUS "** add mysql library")
        #list(APPEND db_link_libs mysqlcppconn8)
    endif()

	if (${PROJECT_NAME}_ODBC_INSTALLED)
		list(APPEND db_link_libs ${UnixODBC_LIBRARIES})
	#        set_property(TARGET cyng_db PROPERTY POSITION_INDEPENDENT_CODE ON)
	endif()
endif(UNIX)
target_link_libraries(cyng_db ${db_link_libs})
#message(STATUS "** db link library   : ${db_link_libs}")

#
#	platform specific libraries
#
if (WIN32)

	include (lib/scm/lib.cmake)
	add_library(cyng_scm ${GLOBAL_LIBRARY_TYPE} ${scm_lib})
	target_include_directories(cyng_scm
		PRIVATE
			"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
			${BOOST_INCLUDE_DIR}
	)

endif()

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
target_include_directories(demo
	PRIVATE
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
)
	
set(CYNG_TARGET_LIST "cyng_async;cyng_csv;cyng_domain;cyng_log;cyng_parser;cyng_store;cyng_vm;cyng_core;cyng_db;cyng_io;cyng_rnd;cyng_sys;cyng_xml;cyng_json;cyng_meta;cyng_sql;cyng_table;cyng_sqlite3")	

if(NOT ${PROJECT_NAME}_CROSS_COMPILE)
	#
	# produce versioned libraries with softlinks 
	# only for native builds. The hassle with the OPKG 
	# toolchain to use symlinks is massive
	#
	set_target_properties(${CYNG_TARGET_LIST}
	PROPERTIES
		SOVERSION ${PROJECT_VERSION_MAJOR}
		VERSION ${PROJECT_VERSION}
	)
endif()

#
# test unit using Boost.Test
# BOOST_TEST_DYN_LINK is required to build a main() function
# cmake -DCYNG_BUILD_TEST:bool=ON ..
#
if(${PROJECT_NAME}_BUILD_TEST AND ${PROJECT_NAME}_SSL_SUPPORT)

    include (test/unit-test/unit-test.cmake)
    add_executable(unit_test ${unit_test})
        
    # BOOST_TEST_DYN_LINK is required to build a main() function
    target_compile_definitions(unit_test
    PRIVATE
		BOOST_TEST_DYN_LINK BOOST_ASIO_HAS_MOVE
    )

	target_include_directories(unit_test
		PRIVATE
			"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
			${PUGIXML_INCLUDE_DIR}
			${BOOST_INCLUDE_DIR}
	)
	
	target_link_libraries(unit_test 
		${CYNG_TARGET_LIST} 
		${PUGIXML_LIBRARY}
		"$<$<PLATFORM_ID:Linux>:${Boost_LIBRARIES};pthread>"
		"$<$<PLATFORM_ID:Windows>:odbc32>"
	)

endif()

#
# unlog - decrypt log files
#
include (tools/unlog/prg.cmake)
add_executable(unlog ${unlog})
target_include_directories(unlog
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
        ${PUGIXML_INCLUDE_DIR}
        ${BOOST_INCLUDE_DIR}
)
target_link_libraries(unlog 
	cyng_core 
	cyng_io
	"$<$<PLATFORM_ID:Linux>:${Boost_LIBRARIES};pthread>"
	)
install(TARGETS unlog
    RUNTIME
    DESTINATION bin)

#
# play - send files to ip address
#
include (tools/play/prg.cmake)
add_executable(play ${play})
target_include_directories(play
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/main/include;${CMAKE_CURRENT_BINARY_DIR}>"
        ${PUGIXML_INCLUDE_DIR}
        ${BOOST_INCLUDE_DIR}
)
target_link_libraries(play 
	cyng_core 
	cyng_io
	"$<$<PLATFORM_ID:Linux>:${Boost_LIBRARIES};pthread>"
)


#
#   Generate documentation
#
include(GenerateDoxygenDoc)

#
# GeneratING OPKG files requires the OPKG tools (https://git.yoctoproject.org/cgit/cgit.cgi/opkg-utils)
# fakeroot opkg/opkg-tools/opkg-buildpackage
# Install with: opkg --force-space install /tmp/oecp-cyng_0.x_armel.ipk
#
if(${PROJECT_NAME}_CROSS_COMPILE)

	string(TOLOWER ${PROJECT_NAME} OECP_NAME)
	
	configure_file (
		  "${PROJECT_SOURCE_DIR}/src/main/templates/opkg/control.in"
		  "${PROJECT_BINARY_DIR}/opkg/control")

	configure_file (
		  "${PROJECT_SOURCE_DIR}/src/main/templates/opkg/postinst.in"
		  "${PROJECT_BINARY_DIR}/opkg/postinst")

	configure_file (
		  "${PROJECT_SOURCE_DIR}/src/main/templates/opkg/rules.in"
		  "${PROJECT_BINARY_DIR}/opkg/rules")
		  
endif()

 
configure_file (
        "${PROJECT_SOURCE_DIR}/src/main/templates/CyngConfig.cmake.in"
        "${PROJECT_SOURCE_DIR}/cmake/CyngConfig.cmake"
)


install(TARGETS ${CYNG_TARGET_LIST} 
    EXPORT CyngTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

#
# Packaging
#
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cyng/ConfigVersion.cmake"
  VERSION "${PROJECT_VERSION}"
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT CyngTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/cyng/CyngTargets.cmake"
  NAMESPACE Cyng::
)
configure_file(cmake/CyngConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/cyng/CyngConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/cyng)
install(EXPORT CyngTargets
  FILE
    CyngTargets.cmake
  NAMESPACE
    Upstream::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    cmake/CyngConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/cyng/CyngConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)
#
# CPack configuration
#
set(CPACK_PACKAGE_VERSION ${VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} - dynamic data types for C++")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(CPACK_PACKAGE_CONTACT "info@solostec.ch")
set(CPACK_PACKAGE_VENDOR "solosTec")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_RPM_PACKAGE_DESCRIPTION "CYNG library")
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/src/assets/LICENSE.txt)

if (${LSB_RELEASE_ID_SHORT} STREQUAL "openSUSE")

	set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
	set(CPACK_GENERATOR "RPM")
	
elseif(${LSB_RELEASE_ID_SHORT} STREQUAL "Ubuntu")	

	set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
	set(CPACK_GENERATOR "DEB")
	
elseif(WIN32)

	set(CPACK_GENERATOR "NSIS64")
	set(CPACK_NSIS_MUI_ICON ${PROJECT_SOURCE_DIR}/src/assets/logo.ico)
	set(CPACK_NSIS_MUI_UNIICON ${PROJECT_SOURCE_DIR}/src/assets/logo.ico)
	#set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q:a'")
	set(CPACK_NSIS_MODIFY_PATH ON)
	set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.solostec.com")

else()
	set(CPACK_GENERATOR "DEB")
endif()


include(CPack)
