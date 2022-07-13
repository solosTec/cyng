# 
#	reset 
#
set (task_lib)

set (task_cpp
    src/channel.cpp
    src/registry.cpp
    src/stash.cpp
    src/scheduler.cpp
    src/controller.cpp
)
    
set (tsk_h
    ${CMAKE_SOURCE_DIR}/include/cyng/task/task_fwd.h
    ${CMAKE_SOURCE_DIR}/include/cyng/task/channel.h
    ${CMAKE_SOURCE_DIR}/include/cyng/task/controller.h
    ${CMAKE_SOURCE_DIR}/include/cyng/task/registry.h
    ${CMAKE_SOURCE_DIR}/include/cyng/task/stash.h
    ${CMAKE_SOURCE_DIR}/include/cyng/task/scheduler.h
    ${CMAKE_SOURCE_DIR}/include/cyng/task/task.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/task/task_interface.h
)

# define the docscript lib
set (task_lib
  ${task_cpp}
  ${tsk_h}
)

