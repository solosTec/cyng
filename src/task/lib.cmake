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
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/task_fwd.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/channel.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/controller.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/registry.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/stash.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/scheduler.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/task.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/task/task_interface.h
)

# define the docscript lib
set (task_lib
  ${task_cpp}
  ${tsk_h}
)

