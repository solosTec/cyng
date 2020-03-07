# - Try to find the pugixml library; Once done this will define
#
# libpugixml-dev 1.7
#
# PUGIXML_INCLUDE_DIR - header location
# PugiXML_LIBRARIES - library to link against
# PugiXML_FOUND - true if pugixml was found.

find_path(PugiXML_INCLUDE_DIRS
	NAMES 
		pugixml.hpp
	PATHS 
		/usr/include
		/usr/local/include
		/usr/include/odbc
		/usr/local/include/odbc
		/usr/include/libodbc
		/usr/local/include/libodbc
		C:/local/pugixml-1.8
)
		
find_library(PugiXML_LIBRARIES
	NAMES 
		pugixml
	PATHS 
		/usr/lib
		/usr/local/lib
		C:/local/pugixml-1.8/scripts
)

#set (PugiXML_VERSION, "1.4")
# Support the REQUIRED and QUIET arguments, and set PUGIXML_FOUND if found.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args( PugiXML 
#	FOUND_VAR PugiXML_FOUND
	REQUIRED_VARS PugiXML_LIBRARIES PugiXML_INCLUDE_DIRS
	VERSION_VAR PugiXML_VERSION
	HANDLE_COMPONENTS
)

if(PugiXML_FOUND)

    set(PUGIXML_VERSION ${PugiXML_VERSION})
    set(PUGIXML_LIBRARY ${PugiXML_LIBRARIES})
    set(PUGIXML_INCLUDE_DIR ${PugiXML_INCLUDE_DIRS})
    
else()
    message(STATUS "No PugiXML found")
endif()

mark_as_advanced(PUGIXML_LIBRARY PUGIXML_INCLUDE_DIR)

