# top level files
set (sqlite_lib)

set (sqlite_cpp

    3party/sqlite-amalgamation-3340000/sqlite3.c  

)

set (sqlite_h

    3party/sqlite-amalgamation-3340000/sqlite3.h 
   
)


# define the SQLite3 library
set (sqlite_lib
  ${sqlite_cpp}
  ${sqlite_h}
)


# 3250000
set(SQLite3_VERSION "3.34.0")
set(SQLite3_INCLUDE_DIR 3party/sqlite-amalgamation-3340000)
set(SQLite3_FOUND ON)

#
# find include file
#
include_directories(${SQLite3_INCLUDE_DIR})
