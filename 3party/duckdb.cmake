# top level files
set (duckdb_lib)

set (duckdb_cpp

    3party/libduckdb-src/duckdb.cpp

)

set (duckdb_h

    3party/libduckdb-src/duckdb.h 
    3party/libduckdb-src/duckdb.hpp
   
)


# define the duckdb3 library
set (duckdb_lib
  ${duckdb_cpp}
  ${duckdb_h}
)


#  DUCKDB_VERSION "v0.8.0"
set(duckdb_VERSION "0.8.0")
set(duckdb_INCLUDE_DIR 3party/libduckdb-src)
set(duckdb_FOUND ON)

#
# find include file
#
include_directories(${duckdb_INCLUDE_DIR})
