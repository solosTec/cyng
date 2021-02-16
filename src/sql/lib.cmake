# 
#	reset 
#
set (sql_lib)

set (sql_cpp
    src/sql/sql.cpp
    src/sql/dialect.cpp
)
    
set (sql_h
    include/cyng/sql/sql.hpp
    include/cyng/sql/dialect.h
    include/cyng/sql/convert.hpp
)

# define the docscript lib
set (sql_lib
  ${sql_cpp}
  ${sql_h}
)

