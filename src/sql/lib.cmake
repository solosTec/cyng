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

set (sql_dsl
    include/cyng/sql/dsl.h
#    include/cyng/sql/dsl/aggregate.hpp
#    include/cyng/sql/dsl/assign.hpp
#    include/cyng/sql/dsl/column.hpp
    include/cyng/sql/dsl/constant.hpp
    include/cyng/sql/dsl/expr_binary.hpp
#    include/cyng/sql/dsl/expr_list.hpp
    include/cyng/sql/dsl/expr_operator.hpp
    include/cyng/sql/dsl/operators.hpp
    include/cyng/sql/dsl/placeholder.h
    include/cyng/sql/dsl/variable.hpp

#    src/sql/aggregate.cpp
#    src/sql/column.cpp
    src/sql/constant.cpp
    src/sql/operators.cpp
    src/sql/placeholder.cpp
    src/sql/variable.cpp
)

source_group("dsl" FILES ${sql_dsl})


# define the docscript lib
set (sql_lib
  ${sql_cpp}
  ${sql_h}
  ${sql_dsl}
)

