# 
#	reset 
#
set (store_lib)

set (store_cpp
    src/key.cpp
    src/meta.cpp
    src/table.cpp
    src/auto_table.cpp
    src/record.cpp
    src/db.cpp
)
    
set (store_h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/key.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/meta.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/table.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/auto_table.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/pub.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/record.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/db.h
)

set (store_signal
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/pub.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/slot.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/store/slot_interface.h
    src/pub.cpp
    src/slot.cpp
)

source_group("signal" FILES ${store_signal})


# define the docscript lib
set (store_lib
  ${store_cpp}
  ${store_h}
  ${store_signal}
)

