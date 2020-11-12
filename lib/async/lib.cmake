# 
#	reset 
#
set (async_lib)

set (async_cpp
 	lib/async/src/pool.cpp
 	lib/async/src/scheduler.cpp
 	lib/async/src/mux.cpp
 	lib/async/src/tmux.cpp
 	lib/async/src/base_task.cpp
 	lib/async/src/signal_handler.cpp
)
    
set (async_h
    src/main/include/cyng/async/policy.h
    src/main/include/cyng/async/pool.h
    src/main/include/cyng/async/scheduler.h
    src/main/include/cyng/async/mux.h
    src/main/include/cyng/async/tmux.h
    src/main/include/cyng/async/task_fwd.h
    src/main/include/cyng/async/signal_handler.h
    src/main/include/cyng/async/task/base_task.h
    src/main/include/cyng/async/task/task.hpp
    src/main/include/cyng/async/task/task_builder.hpp
    src/main/include/cyng/async/task/task_meta.hpp
)


# define the async lib
set (async_lib
  ${async_cpp}
  ${async_h}
)
