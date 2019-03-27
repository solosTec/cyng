
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
# manage unit test: CYNG_BUILD_TEST
# default is OFF
#
if(NOT DEFINED ${PROJECT_NAME}_BUILD_TEST)
	set(${PROJECT_NAME}_BUILD_TEST OFF CACHE BOOL "build unit test")
endif()

#
#	setup C++ compiler
#
if (CMAKE_COMPILER_IS_GNUCXX)

	#
	# gnu C++
	# see: https://stackoverflow.com/questions/36222912/is-c17-stdshared-mutex-not-available-yet
	#
	
	# -std=c++98
	# C++11 since 4.8.1
	# C++14 since 6.1
	# C++17 since 7.0 (?)
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "4.8.1")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++98") 
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		message(STATUS "** C++ support     : C++98")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "5.5")
	# crosscompile OECP with gcc 5.4.0 
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		message(STATUS "** C++ support     : C++11")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "6.1")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		message(STATUS "** C++ support     : C++11")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS  "7.0")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") 
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_OFF)
		message(STATUS "** C++ support     : C++17")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") 
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_OFF)
		message(STATUS "** C++ support     : C++17")
	endif()


	# string_view since 7.3
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER  "7.2")
		set(${PROJECT_NAME}_STD_STRING_VIEW ${PROJECT_NAME}_STD_STRING_VIEW_ON)
    else()
		set(${PROJECT_NAME}_STD_STRING_VIEW ${PROJECT_NAME}_STD_STRING_VIEW_OFF)
	endif()
	
	# shared_mutex experimental support since 6.4 with compiler option C++17
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER  "6.3")
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
	
	#  cross compile with g++ 5.4.0 
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER  "5.5")
		set(${PROJECT_NAME}_STD_PROMISE ${PROJECT_NAME}_STD_PROMISE_ON)
    else()
		set(${PROJECT_NAME}_STD_PROMISE ${PROJECT_NAME}_STD_PROMISE_OFF)
	endif()
	
	set(GLOBAL_LIBRARY_TYPE SHARED)

elseif(MSVC)

	#
	# Microsoft C++
	#
	
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "18.0")
	# 	prior Visual Studio 2013
		message(FATAL_ERROR "Insufficient MSVC version")
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "19.16")
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		set(${PROJECT_NAME}_STD_SHARED_MUTEX ${PROJECT_NAME}_STD_SHARED_MUTEX_ON)
		set(${PROJECT_NAME}_STD_STRING_VIEW	${PROJECT_NAME}_STD_STRING_VIEW_ON)
		message(STATUS "** VC ${CMAKE_CXX_COMPILER_VERSION} supports P0254R2 Integrating string_view And std::string")
		# N4562 Library Fundamentals: <tuple> apply()
		set(${PROJECT_NAME}_STD_APPLY	${PROJECT_NAME}_STD_APPLY_ON)
		message(STATUS "** VC ${CMAKE_CXX_COMPILER_VERSION} supports N4562 Library Fundamentals: <tuple> apply()")
		set(${PROJECT_NAME}_STD_UNIQUE_PTR ${PROJECT_NAME}_STD_UNIQUE_PTR_ON)
		set(${PROJECT_NAME}_STD_PROMISE ${PROJECT_NAME}_STD_PROMISE_ON)
	elseif(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "19.0")
	#	after Visual Studio 2015
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		set(${PROJECT_NAME}_STD_SHARED_MUTEX ${PROJECT_NAME}_STD_SHARED_MUTEX_ON)
		set(${PROJECT_NAME}_STD_STRING_VIEW	${PROJECT_NAME}_STD_STRING_VIEW_OFF)
		#	Even 19.5 lacks support for std::apply()
		set(${PROJECT_NAME}_STD_APPLY	${PROJECT_NAME}_STD_APPLY_OFF)
		set(${PROJECT_NAME}_STD_UNIQUE_PTR ${PROJECT_NAME}_STD_UNIQUE_PTR_ON)
		set(${PROJECT_NAME}_STD_PROMISE ${PROJECT_NAME}_STD_PROMISE_ON)
	else()
		set(${PROJECT_NAME}_LEGACY_MODE ${PROJECT_NAME}_LEGACY_MODE_ON)
		set(${PROJECT_NAME}_STD_SHARED_MUTEX ${PROJECT_NAME}_STD_SHARED_MUTEX_OFF)
		set(${PROJECT_NAME}_STD_STRING_VIEW	${PROJECT_NAME}_STD_STD_STRING_VIEW_OFF)
		set(${PROJECT_NAME}_STD_APPLY	${PROJECT_NAME}_STD_APPLY_OFF)
		set(${PROJECT_NAME}_STD_UNIQUE_PTR ${PROJECT_NAME}_STD_UNIQUE_PTR_OFF)
		set(${PROJECT_NAME}_STD_PROMISE ${PROJECT_NAME}_STD_PROMISE_OFF)
	endif()
	
	set(GLOBAL_LIBRARY_TYPE STATIC)

	#	This is Windows 7 (and rules out windows vista)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /bigobj /std:c++latest /MP")	
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /bigobj /std:c++14 /MP")	
	# When using /std:c++latest use _SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING or _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS too
	# _REMOVE_FPOS_SEEKPOS to remove std::fpos::seekpos entirely. Doesn't work for Boost 1.69.0
	add_definitions(-D_WIN32_WINNT=0x0601 -D_SCL_SECURE_NO_WARNINGS -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE)
	
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
# BOOST_VER is "1_67", "1_68", "1_69" or "1_70"
#
function(windows_boost_fix BOOST_VER)
#  SET(${varName} ${${varName}}${${varName}} PARENT_SCOPE)
  set(BOOST_ROOT "C:/local/boost_${BOOST_VER}_0" CACHE PATH "BOOST_ROOT")
  set(BOOST_LIBRARYDIR "C:/local/boost_${BOOST_VER}_0/lib64-msvc-14.1" CACHE PATH "BOOST_LIBRARYDIR")
  set(BOOST_INCLUDE_DIR "C:/local/boost_${BOOST_VER}_0" CACHE PATH "BOOST_INCLUDE_DIR")
  set(BOOST_DIR "C:/local/boost_${BOOST_VER}" CACHE PATH "BOOST_DIR")
  message(STATUS "** Search Boost    : overwrite with ${BOOST_ROOT}")
  set(Boost_ATOMIC_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_atomic-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_DATE_TIME_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_date_time-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_FILESYSTEM_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_filesystem-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_program_options-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE "${BOOST_LIBRARYDIR}/boost_program_options-vc141-mt-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_RANDOM_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_random-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_REGEX_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_regex-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_SYSTEM_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_system-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_SYSTEM_LIBRARY_RELEASE "${BOOST_LIBRARYDIR}/libboost_system-vc141-mt-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_THREAD_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_thread-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
  set(Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_unit_test_framework-vc141-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)

endfunction()

#
#	setup Boost library 1.70.0, 1.69.0, 1.68.0 or 1.67.0
#
if(NOT ${PROJECT_NAME}_CROSS_COMPILE)
    if(UNIX)
		#
		# search for installation in path ~/projects/boost_X_YY_Z/install
		#
        if(EXISTS "$ENV{HOME}/projects/boost_1_70_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_70_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_70_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_69_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_69_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_69_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_68_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_68_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_68_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_67_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_67_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_67_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_66_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_66_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_66_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        endif()
    elseif(WIN32)
        if(EXISTS "C:/local/boost_1_70_0")
            windows_boost_fix("1_70")
        elseif(EXISTS "C:/local/boost_1_69_0")
            windows_boost_fix("1_69")
        elseif(EXISTS "C:/local/boost_1_68_0")
            windows_boost_fix("1_68")
        elseif(EXISTS "C:/local/boost_1_67_0")
            windows_boost_fix("1_67")
        endif()
    endif(UNIX)
endif()

set(Boost_ADDITIONAL_VERSIONS "1.66.0" "1.67.0" "1.68.0" "1.69.0")
message(STATUS "** Search Boost    : ${Boost_ADDITIONAL_VERSIONS}")
find_package(Boost 1.66 REQUIRED COMPONENTS thread system filesystem program_options random unit_test_framework regex)

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
	# increase MPL list size for Boost.Variant
	# possible values are  30, 40 or 50 
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_SPIRIT_UNICODE -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS -DBOOST_MPL_LIMIT_LIST_SIZE=50 -DFUSION_MAX_VECTOR_SIZE=50")
	
else()
	set(${PROJECT_NAME}_BOOST_LIBRARY ${PROJECT_NAME}_BOOST_LIBRARY_NOT_FOUND)
endif(Boost_FOUND)


#
#	setup SSL library
#
if(NOT DEFINED ${PROJECT_NAME}_SSL_SUPPORT)
#	set(${PROJECT_NAME}_SSL_SUPPORT ON CACHE BOOL "SSL support")
	option(${PROJECT_NAME}_SSL_SUPPORT "SSL support" ON)
endif()


if(${PROJECT_NAME}_SSL_SUPPORT)
    
    message(STATUS "** Search openSSL   : ${OPENSSL_ROOT_DIR}")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 7.0)
        find_package(OpenSSL 1.0.2 REQUIRED)
    else()
        find_package(OpenSSL 1.0.1 REQUIRED)
    endif()
    if(OPENSSL_FOUND)

        add_definitions(-D${PROJECT_NAME}_SSL_INSTALLED)
        
    #	message(STATUS "** openSSL Found         : ${OPENSSL_FOUND}")
        message(STATUS "** openSSL Version       : ${OPENSSL_VERSION}")
        message(STATUS "** openSSL Include       : ${OPENSSL_INCLUDE_DIR}")
        message(STATUS "** openSSL crypto library: ${OPENSSL_CRYPTO_LIBRARY}")
        message(STATUS "** openSSL SSL library   : ${OPENSSL_SSL_LIBRARY}")
        message(STATUS "** openSSL Libraries     : ${OPENSSL_LIBRARIES}")
    
        include_directories(${OPENSSL_INCLUDE_DIR})
        link_directories(${OPENSSL_LIBRARIES})
        
    endif()
    
else()

    message(WARNING "** no SSL support")

endif()

#
#   Don't use a possibly installed SQLite3 library. Instead use
#   version shipped with the repository 3rd library path.
#   SQLite3 library is part of this project
#
include (3party/sqlite.cmake)
add_library(cyng_sqlite3 ${LIBRARY_TYPE} ${sqlite_lib})
set_property(TARGET cyng_sqlite3 PROPERTY POSITION_INDEPENDENT_CODE ON)
add_definitions(-D${PROJECT_NAME}_SQLITE3_INSTALLED)
set(${PROJECT_NAME}_SQLITE3_INSTALLED ON CACHE BOOl "SQLite3 Connector")

#
#
#	SQLite3 shell
include (3party/shell.cmake)
add_executable(sqlite3 ${shell})
if (WIN32)
    target_link_libraries(sqlite3 cyng_sqlite3)
else()
    if(NOT ${PROJECT_NAME}_CROSS_COMPILE)
        target_compile_options(sqlite3 PRIVATE -DHAVE_READLINE)
        target_link_libraries(sqlite3 cyng_sqlite3 pthread ${CMAKE_DL_LIBS} readline)
    else()
        target_link_libraries(sqlite3 cyng_sqlite3 pthread ${CMAKE_DL_LIBS})    
    endif()
endif()
set_property(TARGET sqlite3 PROPERTY LINKER_LANGUAGE C)

#
#
#	Link this library
set(SQLite3_LIBRARY ${lib_sqlite3})

# data comes from sqlite.cmake
message(STATUS "** SQLite3 Version   : ${SQLite3_VERSION}")
message(STATUS "** SQLite3 Include   : ${SQLite3_INCLUDE_DIR}")
message(STATUS "** SQLite3 Libraries : ${SQLite3_LIBRARY}")


#
#	The path to cmake modules
#
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/src/modules/")

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
		set(${PROJECT_NAME}_ODBC_INSTALLED ON CACHE BOOl "ODBC Connector")

	else()

		message(WARNING "** UnixODBC not found")
		set(${PROJECT_NAME}_ODBC_INSTALLED OFF CACHE BOOl "ODBC Connector")

	endif()	

elseif(WIN32)

	#
	#	ODBC is always available on windows
	#
	set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -D${PROJECT_NAME}_ODBC_INSTALLED" )
	set(${PROJECT_NAME}_ODBC_INSTALLED ON CACHE BOOl "ODBC Connector")

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
        set(${PROJECT_NAME}_MYSQL_INSTALLED OFF CACHE BOOl "MySQL Connector")
        message(WARNING "** MySQL Connector not available")
    else()
        set(${PROJECT_NAME}_MYSQL_INSTALLED ON CACHE BOOl "MySQL Connector")
        set(${PROJECT_NAME}_MYSQL_ROOT ${MYSQL_CONNECTOR_ROOT_DIR} CACHE PATH "MYSQL_CONNECTOR_ROOT_DIR")
        message(STATUS "** MySQL Connector available at ${${PROJECT_NAME}_MYSQL_ROOT}")

        set(${PROJECT_NAME}_MYSQL_INCLUDE "${MYSQL_CONNECTOR_ROOT_DIR}/include")
        set(${PROJECT_NAME}_MYSQL_LIB "${MYSQL_CONNECTOR_ROOT_DIR}/build")
        include_directories(${PROJECT_NAME}_MYSQL_INCLUDE)
        link_directories(${PROJECT_NAME}_MYSQL_LIB)
    endif()
else()
    set(${PROJECT_NAME}_MYSQL_INSTALLED OFF CACHE BOOl "MySQL Connector")
    message(STATUS "** MySQL Connector not available")
endif()


#
#   OLEDB is not supported yet
#
set(${PROJECT_NAME}_OLEDB_INSTALLED OFF)

#
# pugixml-master (v1.9)
# On Windows set CMake variables PugiXML_INCLUDE_DIRS and PugiXML_LIBRARIES 
# Included as 3party software
#
set(PUGIXML_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/3party/pugixml-190")
message(STATUS "** PugiXML Include   : ${PUGIXML_INCLUDE_DIR}")
add_definitions(-D${PROJECT_NAME}_PUGIXML_INSTALLED)
include_directories(${PUGIXML_INCLUDE_DIR})
set(${PROJECT_NAME}_PUGIXML_VERSION 190)

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
install(FILES ${meta_lib}
  DESTINATION include)


#
#	cyng core/object library
#
include (lib/core/lib.cmake)
add_library(cyng_core ${GLOBAL_LIBRARY_TYPE} ${core_lib})
if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
	target_link_libraries(cyng_core boost_thread)
endif()
install(TARGETS cyng_core
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

#
#	cyng log library
#
include (lib/log/lib.cmake)
add_library(cyng_log ${GLOBAL_LIBRARY_TYPE} ${log_lib})
install(TARGETS cyng_log
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

#
#	cyng I/O library
#
include (lib/io/lib.cmake)
add_library(cyng_io ${GLOBAL_LIBRARY_TYPE} ${io_lib})
set(io_link_libs cyng_core)
target_link_libraries(cyng_io ${io_link_libs})
install(TARGETS cyng_io
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)


#
#	cyng mail library
#
if(${PROJECT_NAME}_SSL_SUPPORT)
    include (lib/io/mail/lib.cmake)
    add_library(cyng_mail ${GLOBAL_LIBRARY_TYPE} ${mail_lib})
    target_link_libraries(cyng_mail ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})
else()
    message(WARNING "** CYNG mail requires SSL support")
endif()

#
#	cyng VM library
#
include (lib/vm/lib.cmake)
add_library(cyng_vm ${GLOBAL_LIBRARY_TYPE} ${vm_lib})
target_link_libraries(cyng_vm
	cyng_core
	cyng_io)
install(TARGETS cyng_vm
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static)

#
#	cyng async library
#
include (lib/async/lib.cmake)
add_library(cyng_async ${GLOBAL_LIBRARY_TYPE} ${async_lib})

#
#	cyng table library
#
include (lib/table/lib.cmake)
add_library(cyng_table ${GLOBAL_LIBRARY_TYPE} ${table_lib})

#
#	cyng store library
#
include (lib/store/lib.cmake)
add_library(cyng_store ${GLOBAL_LIBRARY_TYPE} ${store_lib})
target_link_libraries(cyng_store
	cyng_core cyng_table)

#
#	cyng domain library
#
include (lib/domain/lib.cmake)
add_library(cyng_domain ${GLOBAL_LIBRARY_TYPE} ${domain_lib})
target_link_libraries(cyng_domain
	cyng_store
	cyng_vm)

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
if(${PROJECT_NAME}_SSL_SUPPORT)
    include (lib/crypto/lib.cmake)
    add_library(cyng_crypto ${GLOBAL_LIBRARY_TYPE} ${crypto_lib})
    target_link_libraries(cyng_crypto
        ${OPENSSL_LIBRARIES})
else()
    message(WARNING "** CYNG crypto requires SSL support")
endif()

#
#	cyng random library
#
include (lib/rnd/lib.cmake)
add_library(cyng_rnd ${GLOBAL_LIBRARY_TYPE} ${rnd_lib})

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
#	cyng CSV library
#
include (lib/csv/lib.cmake)
add_library(cyng_csv ${GLOBAL_LIBRARY_TYPE} ${csv_lib})
target_link_libraries(cyng_csv
	cyng_core
	cyng_io
	cyng_parser)

#
#	cyng SML library
#
include (lib/xml/lib.cmake)
add_library(cyng_xml ${GLOBAL_LIBRARY_TYPE} ${xml_lib})

if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
	target_link_libraries(cyng_xml cyng_parser)
endif()


#
#	cyng docScript library
#
if(${PROJECT_NAME}_SSL_SUPPORT)
	include (lib/docscript/lib.cmake)
	add_library(cyng_docscript ${GLOBAL_LIBRARY_TYPE} ${docscript_lib})
	target_link_libraries(cyng_docscript
		cyng_core
		cyng_async
		cyng_crypto
		cyng_vm
		cyng_json)
endif()

#
#	cyng system library
#
include (lib/sys/lib.cmake)
add_library(cyng_sys ${GLOBAL_LIBRARY_TYPE} ${sys_lib})
# on windows:	-DPSAPI_VERSION=1
if (WIN32)
SET_TARGET_PROPERTIES(cyng_sys PROPERTIES COMPILE_FLAGS "-DPSAPI_VERSION=1")
endif()

#
#	cyng db library
#
include (lib/db/lib.cmake)
add_library(cyng_db ${GLOBAL_LIBRARY_TYPE} ${db_lib})
set(db_link_libs cyng_sqlite3)
if (${PROJECT_NAME}_ODBC_INSTALLED)
	if(WIN32)
		list(APPEND db_link_libs odbc32.lib)
	else()
		list(APPEND db_link_libs ${UnixODBC_LIBRARIES})
#        set_property(TARGET cyng_db PROPERTY POSITION_INDEPENDENT_CODE ON)
 	endif(WIN32)
endif()
if(UNIX)
    list(APPEND db_link_libs ${CMAKE_DL_LIBS} cyng_parser)
    if (${PROJECT_NAME}_MYSQL_INSTALLED)
        #    libmysqlcppconn8.so
        message(STATUS "** add mysql library")
        #list(APPEND db_link_libs mysqlcppconn8)
    endif()
endif(UNIX)
target_link_libraries(cyng_db ${db_link_libs})
message(STATUS "** db link library   : ${db_link_libs}")

#
#	platform specific libraries
#
if (WIN32)

	include (lib/scm/lib.cmake)
	add_library(cyng_scm ${GLOBAL_LIBRARY_TYPE} ${scm_lib})

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
	
#
# test unit using Boost.Test
# BOOST_TEST_DYN_LINK is required to build a main() function
# cmake -DCYNG_BUILD_TEST:bool=ON ..
#
if(${PROJECT_NAME}_BUILD_TEST AND ${PROJECT_NAME}_SSL_SUPPORT)
    include (test/unit-test/unit-test.cmake)
    add_executable(unit_test ${unit_test})
        
    # BOOST_TEST_DYN_LINK is required to build a main() function
    set_property(
        TARGET unit_test
        PROPERTY COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK BOOST_ASIO_HAS_MOVE)

    set(unittest_link_libs cyng_core cyng_io cyng_async cyng_log cyng_store cyng_vm cyng_domain cyng_sql cyng_json cyng_parser cyng_mail cyng_crypto cyng_sys cyng_db cyng_table cyng_xml cyng_csv ${PUGIXML_LIBRARY})

    if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
        list(APPEND unittest_link_libs ${Boost_LIBRARIES})
        if (UNIX)
            list(APPEND unittest_link_libs pthread)
        endif()
    else()
        message(STATUS "** link unit-test        : ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
        list(APPEND unittest_link_libs ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    #	list(APPEND unittest_link_libs ${Boost_LIBRARIES})
    endif()

    #message(STATUS "** unit-test libs    : ${unittest_link_libs}")
    target_link_libraries(unit_test ${unittest_link_libs})
endif()

#
# docScript compiler
#
if(${PROJECT_NAME}_SSL_SUPPORT)
    include (tools/docc/prg.cmake)
    add_executable(docc ${docc})
    set(docc_link_libs cyng_io cyng_vm cyng_async cyng_crypto cyng_log cyng_docscript)
    if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
        if (UNIX)
            list(APPEND docc_link_libs pthread "${Boost_LIBRARIES}")
        endif()
    endif()
    target_link_libraries(docc ${docc_link_libs})
    install(TARGETS docc
        RUNTIME
        DESTINATION bin)


    include (tools/docc/batch/prg.cmake)
    add_executable(batch ${batch})
    set(batch_link_libs cyng_io cyng_vm cyng_async cyng_crypto cyng_log cyng_docscript)
    if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
        if (UNIX)
            list(APPEND batch_link_libs pthread "${Boost_LIBRARIES}")
        endif()
    endif()
    target_link_libraries(batch ${batch_link_libs})
endif()

#
# unlog - decrypt log files
#
include (tools/unlog/prg.cmake)
add_executable(unlog ${unlog})
set(unlog_link_libs cyng_core cyng_io)
if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
	list(APPEND unlog_link_libs "${Boost_LIBRARIES}")
    if (UNIX)
        list(APPEND unlog_link_libs pthread)
    endif()
endif()
target_link_libraries(unlog ${unlog_link_libs})
install(TARGETS unlog
    RUNTIME
    DESTINATION bin)

#
# play - send files to ip address
#
include (tools/play/prg.cmake)
add_executable(play ${play})
set(play_link_libs cyng_core cyng_io)
if (${GLOBAL_LIBRARY_TYPE} STREQUAL "SHARED")
	list(APPEND play_link_libs "${Boost_LIBRARIES}")
    if (UNIX)
        list(APPEND play_link_libs pthread)
    endif()
endif()
target_link_libraries(play ${play_link_libs})


#
# GeneratING OPKG files requires the OPKG tools (https://git.yoctoproject.org/cgit/cgit.cgi/opkg-utils)
# fakeroot opkg/opkg-tools/opkg-buildpackage
# Install with: opkg --force-space install /tmp/oecp-cyng_0.x_armel.ipk
#
if(${PROJECT_NAME}_CROSS_COMPILE)

	configure_file (
		  "${PROJECT_SOURCE_DIR}/src/main/templates/opkg/control"
		  "${PROJECT_BINARY_DIR}/opkg/control")

	configure_file (
		  "${PROJECT_SOURCE_DIR}/src/main/templates/opkg/postinst"
		  "${PROJECT_BINARY_DIR}/opkg/postinst")

	configure_file (
		  "${PROJECT_SOURCE_DIR}/src/main/templates/opkg/rules"
		  "${PROJECT_BINARY_DIR}/opkg/rules")
		  
endif()

#
# CPack configuration
#
if(UNIX)
set(CPACK_PACKAGE_VERSION ${VERSION})

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} - dynamic data types for C++")
set(CPACK_GENERATOR "RPM")

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_RELEASE 1)

set(CPACK_PACKAGE_CONTACT "info@solostec.ch")

set(CPACK_PACKAGE_VENDOR "solosTec")

set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")

include(CPack)
endif()
