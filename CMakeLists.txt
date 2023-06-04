cmake_minimum_required(VERSION 3.14...3.22)

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
    VERSION 0.9.4.${_TWEAK_ID}
    LANGUAGES C CXX
    DESCRIPTION "Dynamic programming in C++"
    HOMEPAGE_URL "https://github.com/solosTec/cyng"
)

#
#   get an uppercase project name
#
string(TOUPPER ${PROJECT_NAME} CAPITAL_NAME)

#
#   convert to false/true that can be used in C/C++ code
#
string(TOLOWER ${CMAKE_CROSSCOMPILING} ${PROJECT_NAME}_CROSSCOMPILING)


# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    #
    # guard against in-source builds and bad build-type strings
    #
    include(cmake/safeguards.cmake)

    math(EXPR "${PROJECT_NAME}_VERSION_NUMERAL" "${PROJECT_VERSION_MAJOR}*100000+${PROJECT_VERSION_MINOR}*1000+${PROJECT_VERSION_PATCH}" OUTPUT_FORMAT DECIMAL)
    message(STATUS "** Version numeral    : ${${PROJECT_NAME}_VERSION_NUMERAL}")

    message(STATUS "** Compiler           : ${CMAKE_CXX_COMPILER_ID} v${CMAKE_CXX_COMPILER_VERSION}")
    foreach(feature ${CMAKE_CXX_COMPILE_FEATURES})
        message(STATUS "** ${feature}")
    endforeach()

    message(STATUS "** CMake              : ${CMAKE_VERSION}")
   
    if (NOT WIN32 AND (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.22.5")) 
        # Alpine Linux v3.16/alpine
        # CMake 3.22.5 required
        cmake_host_system_information(RESULT CYNG_HOST_PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
        cmake_host_system_information(RESULT CYNG_HOST_ID_NAME QUERY DISTRIB_ID)
        message(STATUS "** Host System        : ${CYNG_HOST_PRETTY_NAME}/${CYNG_HOST_ID_NAME} - (${CMAKE_SYSTEM_NAME})")
    else()
        message(STATUS "** Host System        : ${CMAKE_SYSTEM_NAME}")
    endif()

    message(STATUS "** Architecture       : ${CMAKE_SYSTEM_PROCESSOR}")
    

    # Let's ensure -std=c++xx instead of -std=g++xx
    set(CMAKE_CXX_EXTENSIONS OFF)


        # Docs only available if this is the main app
    find_package(Doxygen OPTIONAL_COMPONENTS dot)
    if(Doxygen_FOUND)
        add_subdirectory(docs)
    else()
        message(STATUS "Doxygen not found, not building docs")
    endif()

    if (MSVC)
        message(STATUS "** MSVC_VERSION       : ${MSVC_VERSION}")
        message(STATUS "Use static libraries")
        set(GLOBAL_LIBRARY_TYPE STATIC)
        #set(Boost_USE_STATIC_LIBS ON)
        # target is Windows 10
	    add_compile_definitions(_WIN32_WINNT=0x0A00 _SCL_SECURE_NO_WARNINGS WIN32_LEAN_AND_MEAN)
        add_compile_options(/bigobj)
        add_compile_options(/MP)
        message(STATUS "** compile definitions: ${CMAKE_CXX_FLAGS}")

        #
        #   This is used by PugiXML
        #   Build only a static library
        #
        set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Build only STATIC libraries")

        #
        #   Check if date library is required
        if (MSVC_VERSION LESS 1933)
            message(STATUS "Date library required")
            set(CYNG_USE_DATE_LIBRARY ON CACHE BOOL "use date library")
        else()
            set(CYNG_USE_DATE_LIBRARY OFF CACHE BOOL "use date library")
        endif()

        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "19.22") 
            message(STATUS "Feature testing available in MSVC ${CMAKE_CXX_COMPILER_VERSION}")
            set(CYNG_USE_FEATURE_TESTING ON CACHE BOOL "use feature testing")
        else()
            message(STATUS "Feature testing not available")
            set(CYNG_USE_FEATURE_TESTING OFF CACHE BOOL "use feature testing")
        endif()

    elseif(CMAKE_COMPILER_IS_GNUCXX)
        set(GLOBAL_LIBRARY_TYPE SHARED)

        #
        #   This is used by PugiXML
        #
#        set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build a shared version of library")
        set(BUILD_SHARED_AND_STATIC_LIBS ON CACHE INTERNAL "Build SHARED and STATIC libraries")

        #   Check if date library is required
        if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "9.0") 
            message(STATUS "Feature testing available")
            set(CYNG_USE_FEATURE_TESTING ON CACHE BOOL "use feature testing")
        else()
            message(STATUS "Feature testing not available")
            set(CYNG_USE_FEATURE_TESTING OFF CACHE BOOL "use feature testing")
        endif()

        #
        #   Support for chrono library is a mess. Even gcc 12.2.1 doesn't implements
        #   std::chrono::parse()
        #
        if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS "12.3")
            message(STATUS "Date library required")
            set(CYNG_USE_DATE_LIBRARY ON CACHE BOOL "use date library")
        else()
            message(STATUS "Date library not required")
            set(CYNG_USE_DATE_LIBRARY OFF CACHE BOOL "use date library")
        endif()

    else()
            message(STATUS "unsupported compiler")
            set(CYNG_USE_DATE_LIBRARY ON CACHE BOOL "use date library")
            set(CYNG_USE_FEATURE_TESTING OFF CACHE BOOL "use feature testing")
    endif()

    #
    #   test endian
    #
    include (TestBigEndian)
    TEST_BIG_ENDIAN(${PROJECT_NAME}_BIG_ENDIAN)
    message(STATUS "** big endian         : ${${PROJECT_NAME}_BIG_ENDIAN}")

    #
    # manage unit test: CYNG_UNIT_TEST
    # default is OFF
    #
    if(NOT DEFINED ${CAPITAL_NAME}_UNIT_TEST)
	    set(${CAPITAL_NAME}_UNIT_TEST OFF CACHE BOOL "build unit test")
    endif()
    message(STATUS "** unit tests         : ${${CAPITAL_NAME}_UNIT_TEST}")

endif()

if(NOT GLOBAL_LIBRARY_TYPE)
     if (MSVC)
        set(GLOBAL_LIBRARY_TYPE STATIC)
     else()
        set(GLOBAL_LIBRARY_TYPE SHARED)
     endif()
endif()


#
#   extend search path for CMake modules
#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
message(STATUS "** CMAKE_<CONFIG>_POSTFIX modules     : {${CMAKE_MODULE_PATH}")

#
# detect address model
#
include (cmake/address_model.cmake)

#
# detect available database connectors
#
include (cmake/db_connectors.cmake)



# Find packages ...

set(SMF_BOOST_COMPONENTS regex iostreams program_options system thread random locale)

#
# unit tests
# call CMake with -DCYNG_UNIT_TEST:BOOL=TRUE to generate unit tests 
#
if(${CAPITAL_NAME}_UNIT_TEST)
    message(STATUS "** Generate a Unit-Test")
    list(APPEND SMF_BOOST_COMPONENTS unit_test_framework)
endif()

# Find Boost
set(Boost_NO_BOOST_CMAKE OFF)
find_package(Boost 1.75 
    REQUIRED 
    COMPONENTS ${SMF_BOOST_COMPONENTS} 
)
# note: https://github.com/chriskohlhoff/asio/issues/585
add_compile_definitions(BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE BOOST_BIND_NO_PLACEHOLDERS BOOST_SPIRIT_UNICODE BOOST_MPL_LIMIT_LIST_SIZE=50)

message(STATUS "** Boost Version                : ${Boost_VERSION}")
message(STATUS "** Boost Include                : ${Boost_INCLUDE_DIRS}")
message(STATUS "** Boost Path                   : ${Boost_LIBRARY_DIRS}")
	
if (WIN32 AND ${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24.0")
    #
    # looks like a regression in CMake 3.24.0
    #
    string(REPLACE "debug;" "Debug;" Boost_LOCALE_LIBRARY "${Boost_LOCALE_LIBRARY}")
    string(REPLACE "optimized;" "Release;" Boost_LOCALE_LIBRARY "${Boost_LOCALE_LIBRARY}")
endif()

message(STATUS "** Boost_LIBRARIES              : ${Boost_LIBRARIES}")
message(STATUS "** Boost_THREAD_LIBRARY         : ${Boost_THREAD_LIBRARY}")
message(STATUS "** Boost_SYSTEM_LIBRARY         : ${Boost_SYSTEM_LIBRARY}")
message(STATUS "** Boost_PROGRAM_OPTIONS_LIBRARY: ${Boost_PROGRAM_OPTIONS_LIBRARY}")
message(STATUS "** Boost_RANDOM_LIBRARY         : ${Boost_RANDOM_LIBRARY}")
message(STATUS "** Boost_REGEX_LIBRARY          : ${Boost_REGEX_LIBRARY}")
message(STATUS "** Boost_LOCALE_LIBRARY         : ${Boost_LOCALE_LIBRARY}")
if(${CAPITAL_NAME}_UNIT_TEST)
message(STATUS "** Boost_UNIT_TEST_FRAMEWORK    : ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")
else()
message(STATUS "** Boost_UNIT_TEST_FRAMEWORK    : not installed")
endif()

#
#   List of all targets that will be installed
#
set(REQUESTED_LIBS "cyng_db;cyng_io;cyng_log;cyng_net;cyng_obj;cyng_parse;cyng_rnd;cyng_sql;cyng_store;cyng_sys;cyng_task;cyng_vm;cyng_sqlite3;cyng_xml")
if(WIN32)
    list(APPEND REQUESTED_LIBS "cyng_scm")
endif(WIN32)

#
# external libraries
#

# FetchContent added in CMake 3.11, downloads during the configure step
# FetchContent_MakeAvailable was not added until CMake 3.14
include(FetchContent)

# PTXdist makes this modules available.
# This could be **critical during cross compilation**. Make sure
# that not a local installed library will be found.

find_package(PUGIXML QUIET)

if (PUGIXML_FOUND)
    message(STATUS "** PugiXML          : found") 
    message(STATUS "** PugiXML include  : ${PUGIXML_INCLUDE_DIRS}")
    message(STATUS "** PugiXML library  : ${PUGIXML_LIBRARIES}") 

    if(NOT PTX_BUILD)
        #
        #   Define the target
        #
        message(STATUS "** not building from PTXDist, thus adding pugixml")
        message(STATUS "** add target 'pugixml' ${PUGIXML_LIBRARY}") 
        add_library(pugixml SHARED IMPORTED)
        set_target_properties(pugixml 
            PROPERTIES 
                IMPORTED_LOCATION 
                    ${PUGIXML_LIBRARY}
                INCLUDE_DIRECTORIES
                    ${PUGIXML_INCLUDE_DIRS}
                INTERFACE_INCLUDE_DIRECTORIES
                    ${PUGIXML_INCLUDE_DIRS}
                INTERFACE_SYSTEM_INCLUDE_DIRECTORIES
                    ${PUGIXML_INCLUDE_DIRS}
        )  
    endif(NOT PTX_BUILD)    
else(PUGIXML_FOUND)

    #
    # XML library
    # Could be integrated with find_package() but then the information got lost
    # how PugiXML was build/found.
    #
    message(STATUS "** clone PugiXML ...")
    FetchContent_Declare(
        pugixml
        GIT_REPOSITORY https://github.com/zeux/pugixml
        GIT_TAG v1.11.4
        GIT_PROGRESS TRUE)
    # New in version 3.14
    FetchContent_MakeAvailable(pugixml)
    #FetchContent_Populate(pugixml)
    # Adds pugixml
    # use ${pugixml_SOURCE_DIR}/src
    # /home/.../develop/cyng/aarch64-v8/_deps/pugixml-src
    message(STATUS "** pugixml found                : ${pugixml_POPULATED}")
    message(STATUS "** pugixml library source       : ${pugixml_SOURCE_DIR}")
    message(STATUS "** pugixml build directory      : ${pugixml_BINARY_DIR}")
    set(PUGIXML_FOUND "${pugixml_POPULATED}")
    set(PUGIXML_INCLUDE_DIRS "${pugixml_SOURCE_DIR}/src")
    set(PUGIXML_LIBRARY "${pugixml_BINARY_DIR}")

    #
    #   install pugixml
    #
    list(APPEND REQUESTED_LIBS "pugixml")

endif(PUGIXML_FOUND)


#
# Find Howard Hinnant's date library.
#
if (CYNG_USE_DATE_LIBRARY)
    message(STATUS "** search/download date library...")

    find_package(Date)
    if (DATE_FOUND)
        message(STATUS "** Date source           : ${date_SOURCE_DIR}")

        if(NOT PTX_BUILD)
            add_library(date_src INTERFACE)
            add_library(date::date ALIAS "date_src")
            target_include_directories(date_src 
                INTERFACE 
                    ${date_SOURCE_DIR})
        endif(NOT PTX_BUILD)

    else(DATE_FOUND)

        #
        # date library
        #
        message(STATUS "** clone date ...")
        FetchContent_Declare(
            date_src 
            GIT_REPOSITORY https://github.com/HowardHinnant/date.git
            GIT_TAG v3.0.1
            GIT_PROGRESS TRUE
            CMAKE_CACHE_ARGS -DBUILD_TZ_LIB=OFF)
        FetchContent_MakeAvailable(date_src)
        # Adds date 
        # use ${date_SOURCE_DIR}/include
        message(STATUS "** date library source          : ${date_SOURCE_DIR}")

    endif(DATE_FOUND)
endif(CYNG_USE_DATE_LIBRARY)

#
# add libraries
#
add_subdirectory(src/obj)
add_subdirectory(src/log)
add_subdirectory(src/store)
add_subdirectory(src/sql)
add_subdirectory(src/db)
add_subdirectory(src/task)
add_subdirectory(src/vm)
add_subdirectory(src/io)
add_subdirectory(src/xml)
add_subdirectory(src/parse)
add_subdirectory(src/sys)
add_subdirectory(src/rnd)
add_subdirectory(src/net)

#
#	platform specific libraries
#
if (WIN32)
    add_subdirectory(src/scm)
endif()

if(${CAPITAL_NAME}_UNIT_TEST)
    add_subdirectory(unit_test)
endif()

#
# install
#
if(UNIX)
    include(GNUInstallDirs)
endif()

message(STATUS "** install targets              : ${REQUESTED_LIBS}")
message(STATUS "** CMAKE_INSTALL_DATAROOTDIR    : ${CMAKE_INSTALL_DATAROOTDIR}")


#
# generate config file
#
configure_file (
    "${PROJECT_SOURCE_DIR}/share/cyng.h.in"
    "${PROJECT_BINARY_DIR}/include/${PROJECT_NAME}.h"
)
configure_file (
    "${PROJECT_SOURCE_DIR}/share/version.hpp.in"
    "${PROJECT_SOURCE_DIR}/include/cyng/version.hpp"
)

configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/share/pkgconfig/${PROJECT_NAME}.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
    @ONLY
)

 
install(
    DIRECTORY 
        "${CMAKE_SOURCE_DIR}/include/cyng" # source directory
    DESTINATION 
        "${CMAKE_INSTALL_INCLUDEDIR}" # target directory
        FILES_MATCHING # install only matched files
    PATTERN "*.h"
    PATTERN "*hpp"
)
#
# install cyng.h
#
install(
    FILES 
        "${CMAKE_BINARY_DIR}/include/cyng.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" # target directory
)

#
# install cyng.pc
#
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

# export target to a cyng_coreTargets.cmake
install(TARGETS ${REQUESTED_LIBS}
    EXPORT "${CMAKE_PROJECT_NAME}targets"
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BIN_DIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDE_DIR}
)

#
# Give CMake access to the version
#
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_PROJECT_NAME}ConfigVersion.cmake"
    VERSION 
        "${CMAKE_PROJECT_VERSION}"
    COMPATIBILITY 
        AnyNewerVersion
)

install(EXPORT "${CMAKE_PROJECT_NAME}targets"
  FILE
    ${CMAKE_PROJECT_NAME}targets.cmake
  NAMESPACE
    cyng::
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
)

# install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}Config.cmake"
#               "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}ConfigConfigVersion.cmake"
#     DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake
# )

#
# CPack configuration
#

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_RELEASE "Caboverde")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} - dynamic data types for C++")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_TWEAK ${PROJECT_VERSION_TWEAK})
set(CPACK_PACKAGE_CONTACT "info@solostec.ch")
set(CPACK_PACKAGE_VENDOR "solosTec")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PROJECT_NAME}-${CPACK_PROJECT_VERSION}-${CPACK_PACKAGE_RELEASE}.${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_RPM_PACKAGE_DESCRIPTION "CYNG library")
set(CPACK_RESOURCE_FILE_LICENSE ${PROJECT_SOURCE_DIR}/LICENSE)

if(WIN32)

	set(CPACK_GENERATOR "NSIS64")
	set(CPACK_NSIS_MUI_ICON ${PROJECT_SOURCE_DIR}/src/assets/logo.ico)
	set(CPACK_NSIS_MUI_UNIICON ${PROJECT_SOURCE_DIR}/src/assets/logo.ico)
	#set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "ExecWait '\\\"$INSTDIR\\\\vcredist_x86.exe\\\" /q:a'")
	set(CPACK_NSIS_MODIFY_PATH ON)
	set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.solostec.com")

else()

    set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

    if (DEFINED CYNG_HOST_ID_NAME)
        # opensuse-tumbleweed
        if (${CYNG_HOST_ID_NAME} MATCHES "opensuse*")
            set(CPACK_RPM_PACKAGE_NAME ${PROJECT_NAME})
            set(CPACK_RPM_PACKAGE_SUMMARY "${PROJECT_NAME} - dynamic data types for C++")
            set(CPACK_RPM_PACKAGE_MAINTAINER "S. Olzscher")
            set(CPACK_PACKAGE_VENDOR "solosTec")
            set(CPACK_RPM_PACKAGE_URL "solostec.com")
            set(CPACK_GENERATOR "RPM")
        endif()
    else()    
        #set(CPACK_DEBIAN_PACKAGE_DEPENDS "libboost-all-dev (>= 1.76), openssl (>= 1.1.0)")

        #   architecture: amd64
        set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
        SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "solosTec") 
	    set(CPACK_GENERATOR "DEB")
    endif()
endif()

include(CPack)
