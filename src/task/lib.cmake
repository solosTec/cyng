# 
#	reset 
#
set (task_lib)

set (task_cpp
    src/task/channel.cpp
    src/task/registry.cpp
    src/task/scheduler.cpp
    src/task/controller.cpp
)
    
set (tsk_h
    include/cyng/task/task_fwd.h
    include/cyng/task/channel.h
    include/cyng/task/controller.h
    include/cyng/task/registry.h
    include/cyng/task/scheduler.h
    include/cyng/task/task.hpp
    include/cyng/task/task_interface.h
)

# define the docscript lib
set (task_lib
  ${task_cpp}
  ${tsk_h}
)

