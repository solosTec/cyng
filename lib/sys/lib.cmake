# 
#	reset 
#
set (sys_lib)

set (sys_cpp
  	lib/sys/src/memory.cpp
  	lib/sys/src/cpu.cpp
)
    
set (sys_h
	src/main/include/cyng/sys/memory.h
	src/main/include/cyng/sys/cpu.h
)

# define the sys lib
set (sys_lib
  ${sys_cpp}
  ${sys_h}
)
