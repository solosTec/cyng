# 
#	reset 
#
set (sql_lib)

set (sql_cpp
    src/sql.cpp
    src/dialect.cpp
)
    
set (sql_h
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/sql.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dialect.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/convert.hpp
)

set (sql_dsl
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl.h
#    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/aggregate.hpp
#    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/assign.hpp
#    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/column.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/constant.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/expr_binary.hpp
#    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/expr_list.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/expr_operator.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/operators.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/placeholder.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sql/dsl/variable.hpp

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

