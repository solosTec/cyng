# FindCYNG.cmake
#
# Finds the CYNG library
#
# This will define the following variables
#
#    CYNG_FOUND
#    CYNG_INCLUDE_DIRS
#
# and the following imported targets
#
#     CYNG::CYNG
#

find_package(PkgConfig)
pkg_check_modules(PC_CYNG QUIET CYNG)

find_path(CYNG_INCLUDE_DIR
    NAMES cyng.h
    PATHS ${PC_CYNG_INCLUDE_DIRS}
    PATH_SUFFIXES cyng
)

set(CYNG_VERSION ${PC_CYNG_VERSION})

mark_as_advanced(CYNG_FOUND CYNG_INCLUDE_DIR CYNG_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CYNG
    REQUIRED_VARS CYNG_INCLUDE_DIR
    VERSION_VAR CYNG_VERSION
)

if(CYNG_FOUND)
    set(CYNG_INCLUDE_DIRS ${CYNG_INCLUDE_DIR})
endif()

if(CYNG_FOUND AND NOT TARGET CYNG::CYNG)
    add_library(CYNG::CYNG INTERFACE IMPORTED)
    set_target_properties(CYNG::CYNG PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CYNG_INCLUDE_DIR}"
    )
endif()