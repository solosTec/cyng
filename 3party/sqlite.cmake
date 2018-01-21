# top level files
set (sqlite_lib)

set (sqlite_cpp

    3party/sqlite-amalgamation-3210000/sqlite3.c  

)

set (sqlite_h

    3party/sqlite-amalgamation-3210000/sqlite3.h 
   
)


# define the SQLite3 library
set (sqlite_lib
  ${sqlite_cpp}
  ${sqlite_h}
)

#
# find include file
#
include_directories(3party/sqlite-amalgamation-3210000)

# 3210000
set(SQLite3_VERSION "3.21.0")
set(SQLite3_FOUND ON)
