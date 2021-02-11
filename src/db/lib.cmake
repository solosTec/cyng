# 
#	reset 
#
set (db_lib)

set (db_cpp
    src/db/session.cpp
    src/db/connection_types.cpp
)
    
set (db_h
    include/cyng/db/session.h
    include/cyng/db/connection_types.h
)

# define the docscript lib
set (db_lib
  ${db_cpp}
  ${db_h}
)

