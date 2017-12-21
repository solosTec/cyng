# 
#	reset 
#
set (sql_lib)

set (sql_cpp
 	lib/sql/src/sql.cpp
 	lib/sql/src/dialect.cpp
)
    
set (sql_h
    src/main/include/cyng/sql.h
    src/main/include/cyng/sql/dialect.h
)

set (sql_dsl
    src/main/include/cyng/sql/dsl/binary_expr.hpp
    src/main/include/cyng/sql/dsl/list_expr.hpp
    src/main/include/cyng/sql/dsl/operators.hpp
    src/main/include/cyng/sql/dsl/variable.hpp
    src/main/include/cyng/sql/dsl/constant.hpp
 	lib/sql/src/constant.cpp
    src/main/include/cyng/sql/dsl/column.h
 	lib/sql/src/column.cpp
    src/main/include/cyng/sql/dsl/placeholder.h
 	lib/sql/src/placeholder.cpp
    src/main/include/cyng/sql/dsl/aggregate.h
 	lib/sql/src/aggregate.cpp
)

# define the sql lib
set (sql_lib
  ${sql_cpp}
  ${sql_h}
  ${sql_dsl}
)
