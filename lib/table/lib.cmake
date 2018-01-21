# 
#	reset 
#
set (table_lib)

set (table_cpp
 	lib/table/src/meta.cpp
 	lib/table/src/record.cpp
# 	lib/table/src/publisher.cpp
# 	lib/table/src/table.cpp
# 	lib/table/src/db.cpp
 	lib/table/src/key.cpp
)
    
set (table_h
    src/main/include/cyng/table/meta_interface.h
    src/main/include/cyng/table/meta.hpp
    src/main/include/cyng/table/key.hpp
    src/main/include/cyng/table/body.hpp
    src/main/include/cyng/table/record.h
#    src/main/include/cyng/table/publisher.h
#    src/main/include/cyng/table/table.h
#    src/main/include/cyng/table/db.h
#	src/main/include/cyng/table/transform.hpp
)

# define the table lib
set (table_lib
  ${table_cpp}
  ${table_h}
)
