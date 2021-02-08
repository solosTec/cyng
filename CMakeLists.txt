cmake_minimum_required(VERSION 3.14...3.19)

#
# from command line: 
# cmake -DBOOST_ROOT=C:\\local\\boost_1_75_0 -G "Visual Studio 16 2019" -A x64 ..
# cmake -DBOOST_ROOT=/home/sol/projects/install/x64/boost -G "Ninja" ..
#

#
# get time stamp of this build
# produce variables _THIS_YEAR and _TWEAK_ID
#
include (cmake/time_stamp.cmake)

project(
    cyng
    VERSION 0.9.0.${_TWEAK_ID}
    LANGUAGES CXX
)


# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    math(EXPR "${PROJECT_NAME}_VERSION_NUMERAL" "${PROJECT_VERSION_MAJOR}*100000+${PROJECT_VERSION_MINOR}*1000+${PROJECT_VERSION_PATCH}" OUTPUT_FORMAT DECIMAL)
    message(STATUS "** Version numeral    : ${${PROJECT_NAME}_VERSION_NUMERAL}")

    message(STATUS "** Compiler           : ${CMAKE_CXX_COMPILER_ID} v${CMAKE_CXX_COMPILER_VERSION}")


    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)

    set(CPACK_PROJECT_NAME ${PROJECT_NAME})
    set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
    include(CPack)


    # Docs only available if this is the main app
    find_package(Doxygen)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()

    if (WIN32)
        message(STATUS "Use static libraries")
        set(GLOBAL_LIBRARY_TYPE STATIC)
        #set(Boost_USE_STATIC_LIBS ON)
        # target is Windows 10
	    add_compile_definitions(_WIN32_WINNT=0x0A00 _SCL_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN)
        add_compile_options("/bigobj")
        message(STATUS "** compile definitions: ${CMAKE_CXX_FLAGS}")

    else()
        set(GLOBAL_LIBRARY_TYPE SHARED)
    endif()

    #
    #   test endian
    #
    include (TestBigEndian)
    TEST_BIG_ENDIAN(${PROJECT_NAME}_BIG_ENDIAN)
    message(STATUS "** big endian         : ${${PROJECT_NAME}_BIG_ENDIAN}")

endif()

#
# detect address model
#
include (cmake/address_model.cmake)

#
#   convert to false/true that can be used in C/C++ code
#
string(TOLOWER ${CMAKE_CROSSCOMPILING} ${PROJECT_NAME}_CROSSCOMPILING)

#   use shared libraries
set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build SHARED libraries")

# Find packages ...

# Find Boost
set(Boost_NO_BOOST_CMAKE OFF)
find_package(Boost 1.75 
    REQUIRED 
    COMPONENTS regex iostreams program_options unit_test_framework system thread random date_time
#    PATHS "C:/local/boost_1_75_0"
#    CONFIG
)
# note: https://github.com/chriskohlhoff/asio/issues/585
add_compile_definitions(BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE BOOST_BIND_NO_PLACEHOLDERS BOOST_SPIRIT_UNICODE BOOST_MPL_LIMIT_LIST_SIZE=50)

message(STATUS "** Boost Version    : ${Boost_VERSION}")
message(STATUS "** Boost Include    : ${Boost_INCLUDE_DIRS}")
message(STATUS "** Boost Path       : ${Boost_LIBRARY_DIRS}")
	
message(STATUS "** Boost_LIBRARIES              : ${Boost_LIBRARIES}")
message(STATUS "** Boost_THREAD_LIBRARY         : ${Boost_THREAD_LIBRARY}")
message(STATUS "** Boost_SYSTEM_LIBRARY         : ${Boost_SYSTEM_LIBRARY}")
#message(STATUS "** Boost_FILESYSTEM_LIBRARY     : ${Boost_FILESYSTEM_LIBRARY}")
message(STATUS "** Boost_DATE_TIME_LIBRARY      : ${Boost_DATE_TIME_LIBRARY}")
message(STATUS "** Boost_PROGRAM_OPTIONS_LIBRARY: ${Boost_PROGRAM_OPTIONS_LIBRARY}")
message(STATUS "** Boost_RANDOM_LIBRARY         : ${Boost_RANDOM_LIBRARY}")
message(STATUS "** Boost_REGEX_LIBRARY          : ${Boost_REGEX_LIBRARY}")
message(STATUS "** Boost_UNIT_TEST_FRAMEWORK    : ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")


# FetchContent added in CMake 3.11, downloads during the configure step
include(FetchContent)
# FetchContent_MakeAvailable was not added until CMake 3.14; use our shim
# if(${CMAKE_VERSION} VERSION_LESS 3.14)
#   include(cmake/add_FetchContent_MakeAvailable.cmake)
# endif()

# XML library
#
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml
  GIT_TAG v1.11)
FetchContent_MakeAvailable(pugixml)
# Adds pugixml

# chrono library
#
FetchContent_Declare(
  date_src 
  GIT_REPOSITORY https://github.com/HowardHinnant/date.git
  GIT_TAG v3.0.0)
FetchContent_MakeAvailable(date_src)
# Adds date 

# SQLite3 library
#
find_package (SQLite3)
if (SQLITE3_FOUND)
    message(STATUS "SQLite3 ${SQLite3_INCLUDE_DIRS}")
endif()
# target_link_libraries (${OUT_TARGET} SQLite3::SQLite3)

#   http://pqxx.org/development/libpqxx/
#
find_library(PQXX_LIB pqxx)
find_library(PQ_LIB pq)
if (PQXX_LIB_FOUND)
#
#   add libpqxx to the available database connectors
#
    message(STATUS "** PostgreSQL  : <LANG>_VISIBILITY_PRESET available")

endif()

#
# add libraries
#

#
# add library: cyng_obj
#
include (src/obj/lib.cmake)
add_library(cyng_obj ${GLOBAL_LIBRARY_TYPE} ${object_lib})

target_include_directories(cyng_obj
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_obj PRIVATE 
cxx_alignas
    cxx_std_20
#    cxx_alignof
#    cxx_attributes
    cxx_auto_type
    cxx_constexpr
#    cxx_defaulted_functions
#    cxx_deleted_functions
#    cxx_final
    cxx_lambdas
    cxx_noexcept
    cxx_override
    cxx_range_for
    cxx_rvalue_references
    cxx_static_assert
    cxx_strong_enums
#    cxx_trailing_return_types
#    cxx_unicode_literals
#    cxx_user_literals
#    cxx_variadic_macros
)

#
#   debug class object
#
target_compile_definitions(cyng_obj 
    PRIVATE 
#        $<$<CONFIG:Debug>:_DEBUG_OBJECT>
)

target_link_libraries(cyng_obj 
    PRIVATE 
        Boost::boost 
        date::date
)

#
# add library: cyng_vm
#
include (src/vm/lib.cmake)
add_library(cyng_vm ${GLOBAL_LIBRARY_TYPE} ${vm_lib})

target_include_directories(cyng_vm
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_vm 
    PRIVATE 
        cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug class object
#
target_compile_definitions(cyng_vm 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_OBJECT>
        $<$<CONFIG:Debug>:_DEBUG_VM>
)

target_link_libraries(cyng_vm 
    PRIVATE 
        Boost::boost 
)

#
# add library: cyng_task
#
include (src/task/lib.cmake)
add_library(cyng_task ${GLOBAL_LIBRARY_TYPE} ${task_lib})

target_include_directories(cyng_task
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_task PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug class object
#
target_compile_definitions(cyng_task 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_OBJECT>
        $<$<AND:$<VERSION_LESS_EQUAL:${CMAKE_CXX_COMPILER_VERSION},9.0>,$<CXX_COMPILER_ID:GNU>>:BOOST_ASIO_HAS_STD_FUTURE>       
)

target_link_libraries(cyng_task 
    PRIVATE 
        Boost::boost 
)

#
# add library: cyng_store
#
include (src/store/lib.cmake)
add_library(cyng_store ${GLOBAL_LIBRARY_TYPE} ${store_lib})

target_include_directories(cyng_store
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_store PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug class object
#
target_compile_definitions(cyng_store 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_STORE>
)
#
# -Wno-psabi for gcc > 7.1
#
target_compile_options(cyng_store 
    PRIVATE 
        $<$<AND:$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},7.1>,$<CXX_COMPILER_ID:GNU>>:-Wno-psabi>
)

target_link_libraries(cyng_store 
    PRIVATE 
        Boost::boost 
)

#
# add library: cyng_io
#
include (src/io/lib.cmake)
add_library(cyng_io ${GLOBAL_LIBRARY_TYPE} ${io_lib})

target_include_directories(cyng_io
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_io PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

target_link_libraries(cyng_io 
    PRIVATE 
        Boost::boost 
)

#
# add library: cyng_log
#
include (src/log/lib.cmake)
add_library(cyng_log ${GLOBAL_LIBRARY_TYPE} ${log_lib})

target_include_directories(cyng_log
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_log PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug logger
#
target_compile_definitions(cyng_log 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_LOG>
)

target_link_libraries(cyng_log 
    PRIVATE 
        Boost::boost 
)

#
# add library: cyng_sys
#
include (src/sys/lib.cmake)
add_library(cyng_sys ${GLOBAL_LIBRARY_TYPE} ${sys_lib})

target_include_directories(cyng_sys
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_sys PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug sys library
#
target_compile_definitions(cyng_sys 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_SYS>
        $<IF:$<BOOL:${CMAKE_CROSSCOMPILING}>,__CROSS_PLATFORM,__NATIVE_PLATFORM>
)

target_link_libraries(cyng_sys 
    PRIVATE 
        Boost::boost 
)
  
#
# add library: cyng_parse
#
include (src/parse/lib.cmake)
add_library(cyng_parse ${GLOBAL_LIBRARY_TYPE} ${parse_lib})

target_include_directories(cyng_parse
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_parse PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug parse library
#
target_compile_definitions(cyng_parse 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_PARSE>
)

target_link_libraries(cyng_parse 
    PRIVATE 
        Boost::boost 
)

#
# add library: cyng_rnd
#
include (src/rnd/lib.cmake)
add_library(cyng_rnd ${GLOBAL_LIBRARY_TYPE} ${rnd_lib})

target_include_directories(cyng_rnd
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_rnd PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug rnd library
#
target_compile_definitions(cyng_rnd 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_RND>
)

target_link_libraries(cyng_rnd 
    PRIVATE 
        Boost::boost 
)

#
# add library: cyng_sql
#
include (src/sql/lib.cmake)
add_library(cyng_sql ${GLOBAL_LIBRARY_TYPE} ${sql_lib})

target_include_directories(cyng_sql
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_sql PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug sql library
#
target_compile_definitions(cyng_sql 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_SQL>
)

target_link_libraries(cyng_sql 
    PRIVATE 
        Boost::boost 
)

#
# unit tests
#
add_subdirectory(unit_test)

#
# generate config file
#
configure_file (
    "${PROJECT_SOURCE_DIR}/include/cyng.h.in"
    "${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}.h"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/include/version.hpp.in"
    "${PROJECT_SOURCE_DIR}/include/cyng/version.hpp"
)

#
# install
#

# export target to a cyng_coreTargets.cmake
include(GNUInstallDirs)
install(TARGETS cyng_obj
    EXPORT cyng-export
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT cyng-export
  FILE
    cyng_coretargets.cmake
  NAMESPACE
    cyng::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/cyng
)
