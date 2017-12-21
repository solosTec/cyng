# 
#	reset 
#
set (store_lib)

set (store_cpp
 	lib/store/src/record.cpp
 	lib/store/src/publisher.cpp
 	lib/store/src/table.cpp
 	lib/store/src/db.cpp
 	lib/store/src/key.cpp
)
    
set (store_h
    src/main/include/cyng/store/meta_interface.h
    src/main/include/cyng/store/meta.hpp
    src/main/include/cyng/store/key.hpp
    src/main/include/cyng/store/body.hpp
    src/main/include/cyng/store/record.h
    src/main/include/cyng/store/publisher.h
    src/main/include/cyng/store/table.h
    src/main/include/cyng/store/db.h
)

# define the store lib
set (store_lib
  ${store_cpp}
  ${store_h}
)
