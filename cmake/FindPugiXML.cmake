# Find the pugixml XML parsing library.
#
# Sets the usual variables expected for find_package scripts:
#
# PUGIXML_INCLUDE_DIR - header location
# PUGIXML_LIBRARIES - library to link against
# PUGIXML_FOUND - true if pugixml was found.

find_path(PUGIXML_INCLUDE_DIR 
    NAMES 
        pugixml.hpp 
        pugiconfig.hpp
    PATH_SUFFIXES
        include
    HINTS 
        "/usr"
)


find_library(PUGIXML_LIBRARY 
    NAMES 
        pugixml 
    PATH_SUFFIXES
        "/usr/lib/x86_64-linux-gnu/"
        "lib64"
        "lib"
    HINTS 
        "/usr/lib/x86_64-linux-gnu/"
)

# Support the REQUIRED and QUIET arguments, and set PUGIXML_FOUND if found.
include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(PugiXML DEFAULT_MSG PUGIXML_LIBRARY PUGIXML_INCLUDE_DIR)

if (PUGIXML_FOUND)
    set (PUGIXML_LIBRARIES ${PUGIXML_LIBRARY})
    if (NOT PugiXML_FIND_QUIETLY)
        message(STATUS "** PugiXML include       : ${PUGIXML_INCLUDE_DIR}")
        message(STATUS "** PugiXML library       : ${PUGIXML_LIBRARY}") 
    endif()
else (PUGIXML_FOUND)
    message(STATUS "** PugiXML not found")
endif(PUGIXML_FOUND)

mark_as_advanced (PUGIXML_LIBRARY PUGIXML_INCLUDE_DIR)

#
#   layout on ubuntu 22.x
#
#/usr/lib/x86_64-linux-gnu/pkgconfig/pugixml.pc
#/usr/lib/x86_64-linux-gnu/cmake/pugixml/pugixml-targets.cmake
#/usr/lib/x86_64-linux-gnu/cmake/pugixml/pugixml-targets-none.cmake
#/usr/lib/x86_64-linux-gnu/cmake/pugixml/pugixml-config-version.cmake
#/usr/lib/x86_64-linux-gnu/cmake/pugixml/pugixml-config.cmake
#/usr/include/pugixml.hpp
#/usr/include/pugiconfig.hpp
#/usr/lib/x86_64-linux-gnu/libpugixml.so.1.12
#/usr/lib/x86_64-linux-gnu/libpugixml.so.1
#/usr/lib/x86_64-linux-gnu/libpugixml.a
#/usr/lib/x86_64-linux-gnu/libpugixml.so

