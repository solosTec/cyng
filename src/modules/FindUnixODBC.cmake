# - Try to find the UnixODBC library; Once done this will define
#
# UnixODBC_FOUND - system has the UnixODBC library
# UnixODBC_INCLUDE_DIRS - the UnixODBC include directory
# UnixODBC_LIBRARIES - The libraries needed to use UnixODBC

# Look for the header file
FIND_PATH(UnixODBC_INCLUDE_DIR uodbc_stats.h
	/usr/include
	/usr/local/include
	/usr/include/odbc
	/usr/local/include/odbc
	/usr/include/libodbc
	/usr/local/include/libodbc
)

# Look for the library
find_library(UnixODBC_LIBRARY
	NAMES
		odbc
	PATHS
		/usr/lib
		/usr/local/lib)

# Copy the results to the output variables.
if(UnixODBC_INCLUDE_DIR AND UnixODBC_LIBRARY)
	set(UnixODBC_FOUND 1)
	set(UnixODBC_LIBRARIES ${UnixODBC_LIBRARY})
	set(UnixODBC_INCLUDE_DIRS ${UnixODBC_INCLUDE_DIR})
else(UnixODBC_INCLUDE_DIR AND UnixODBC_LIBRARY)
	set(UnixODBC_FOUND 0)
	set(UnixODBC_LIBRARIES)
	set(UnixODBC_INCLUDE_DIRS)
endif(UnixODBC_INCLUDE_DIR AND UnixODBC_LIBRARY)

# Report the results.
if(NOT UnixODBC_FOUND)
	set(UnixODBC_DIR_MESSAGE "UnixODBC was not found. Make sure UnixODBC_LIBRARY and UnixODBC_INCLUDE_DIR are set.")
	if(NOT UnixODBC_FIND_QUIETLY)
		message(STATUS "${UnixODBC_DIR_MESSAGE}")
	else(NOT UnixODBC_FIND_QUIETLY)
		if(UnixODBC_FIND_REQUIRED)
			message(FATAL_ERROR "${UnixODBC_DIR_MESSAGE}")
		endif(UnixODBC_FIND_REQUIRED)
	endif(NOT UnixODBC_FIND_QUIETLY)
endif(NOT UnixODBC_FOUND)

mark_as_advanced(UnixODBC_INCLUDE_DIRS)
mark_as_advanced(UnixODBC_LIBRARIES)

