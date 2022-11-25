# 
#	reset 
#
set (sql_lib)

set (sql_cpp
    src/sql.cpp
    src/dialect.cpp
)
    
set (sql_h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/sql.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dialect.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/convert.hpp
)

set (sql_dsl
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl.h
#    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/aggregate.hpp
#    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/assign.hpp
#    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/column.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/constant.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/expr_binary.hpp
#    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/expr_list.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/expr_operator.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/operators.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/placeholder.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sql/dsl/variable.hpp

#    src/aggregate.cpp
#    src/column.cpp
    src/constant.cpp
    src/operators.cpp
    src/placeholder.cpp
    src/variable.cpp
)

source_group("dsl" FILES ${sql_dsl})


# define the docscript lib
set (sql_lib
  ${sql_cpp}
  ${sql_h}
  ${sql_dsl}
)

