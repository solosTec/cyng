# 
#	reset 
#
set (table_lib)

set (table_cpp
 	lib/table/src/meta.cpp
 	lib/table/src/record.cpp
 	lib/table/src/key.cpp
 	lib/table/src/restore.cpp
)
    
set (table_h
    src/main/include/cyng/table/table_fwd.h
    src/main/include/cyng/table/meta_interface.h
    src/main/include/cyng/table/meta.hpp
    src/main/include/cyng/table/key.hpp
    src/main/include/cyng/table/body.hpp
    src/main/include/cyng/table/record.h
    src/main/include/cyng/table/restore.h
)

# define the table lib
set (table_lib
  ${table_cpp}
  ${table_h}
)
