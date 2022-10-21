# Find Howard Hinnant's date library.
#
# This is a header only library.
# Sets the usual variables expected for find_package scripts:
#
# date_SOURCE_DIR - header location
# DATE_FOUND - true if date was found.

find_path(date_SOURCE_DIR 
    NAMES 
        "date/date.h"
    PATHS 
        "/usr"
        "/home/sol/develop/x64/date"
    PATH_SUFFIXES
        "include"
)

# Support the REQUIRED and QUIET arguments, and set DATE_FOUND if found.
include (FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Date DEFAULT_MSG date_SOURCE_DIR)

if (DATE_FOUND)
    if (NOT DateFIND_QUIETLY)
        message(STATUS "** Date include          : ${date_SOURCE_DIR}")
    endif()
else (DATE_FOUND)
    message(STATUS "** Date not found")
endif(DATE_FOUND)

mark_as_advanced (date_SOURCE_DIR)

#
#   layout on ubuntu 22.x
#
