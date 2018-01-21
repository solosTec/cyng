# CMake module to search for Sqlite3 library
#
# If it's found it sets SQLite3_FOUND to TRUE
# and following variables are set:
#    SQLite3_INCLUDE_DIR
#    SQLite3_LIBRARY

if (SQLite3_LIBRARIES AND SQLite3_INCLUDE_DIRS)

	# in cache already
	set(SQLite3_FOUND TRUE)
	
else (SQLite3_LIBRARIES AND SQLite3_INCLUDE_DIRS)

	# use pkg-config to get the directories and then use these values
	# in the FIND_PATH() and FIND_LIBRARY() calls
	find_package(PkgConfig)
	if (PKG_CONFIG_FOUND)
		pkg_check_modules(_SQLITE3 sqlite3)
	endif (PKG_CONFIG_FOUND)
	
	find_path(SQLite3_INCLUDE_DIR
	NAMES
		sqlite3.h
	PATHS
		${_SQLITE3_INCLUDEDIR}
		/usr/include
		/usr/local/include
		/opt/local/include
		/sw/include
		$ENV{LIB_DIR}/include
		$ENV{LIB_DIR}/include/sqlite
		$ENV{LIB_DIR}/include/sqlite3
		$ENV{ProgramFiles}/SQLite/*/include
		$ENV{ProgramFiles}/SQLite3/*/include
		$ENV{SystemDrive}/SQLite/*/include
		$ENV{SystemDrive}/SQLite3/*/include
	)

	find_library(SQLite3_LIBRARY
	NAMES
		sqlite3
	PATHS
		${_SQLITE3_LIBDIR}
		/usr/lib
		/usr/local/lib
		/usr/lib/x86_64-linux-gnu
		/opt/local/lib
		/sw/lib
		$ENV{LIB_DIR}/lib
		$ENV{ProgramFiles}/SQLite/*/lib
		$ENV{ProgramFiles}/SQLite3/*/lib
		$ENV{SystemDrive}/SQLite/*/lib
		$ENV{SystemDrive}/SQLite3/*/lib
	)

	if (SQLite3_LIBRARY)
		set(SQLite3_FOUND TRUE)
		set(SQLite3_VERSION ${_SQLITE3_VERSION})
	endif()

	set(SQLite3_INCLUDE_DIRS ${SQLite3_INCLUDE_DIR})

	if (SQLite3_FOUND)
		set(SQLite3_LIBRARIES ${SQLite3_LIBRARIES} ${SQLite3_LIBRARY})
	endif (SQLite3_FOUND)

	if (!SQLite3_FOUND AND Sqlite3_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find Sqlite3")
	endif()

	# show the SQLite3_INCLUDE_DIRS and SQLite3_LIBRARIES variables only in the advanced view
	mark_as_advanced(SQLite3_INCLUDE_DIRS SQLite3_LIBRARIES)

endif(SQLite3_LIBRARIES AND SQLite3_INCLUDE_DIRS)
