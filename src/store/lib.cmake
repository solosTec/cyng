# 
#	reset 
#
set (store_lib)

set (store_cpp
    src/store/key.cpp
    src/store/meta.cpp
    src/store/table.cpp
    src/store/auto_table.cpp
    src/store/pub.cpp
    src/store/record.cpp
    src/store/db.cpp
)
    
set (store_h
    include/cyng/store/key.hpp
    include/cyng/store/meta.h
    include/cyng/store/table.h
    include/cyng/store/auto_table.h
    include/cyng/store/pub.h
    include/cyng/store/record.h
    include/cyng/store/db.h
)

# define the docscript lib
set (store_lib
  ${store_cpp}
  ${store_h}
)

