# 
#	reset 
#
set (sys_lib)

set (sys_cpp
    src/sys/host.cpp
    src/sys/process.cpp
    src/sys/memory.cpp
)
    
set (sys_h
    include/cyng/sys/host.h
    include/cyng/sys/process.h
    include/cyng/sys/memory.h
)

# define the docscript lib
set (sys_lib
  ${sys_cpp}
  ${sys_h}
)

