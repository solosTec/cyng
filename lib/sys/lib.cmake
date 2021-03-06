# 
#	reset 
#
set (sys_lib)

set (sys_cpp
  	lib/sys/src/memory.cpp
  	lib/sys/src/cpu.cpp
  	lib/sys/src/process.cpp
  	lib/sys/src/mac.cpp
  	lib/sys/src/info.cpp
  	lib/sys/src/fsys.cpp
)
    
set (sys_h
	src/main/include/cyng/sys/memory.h
	src/main/include/cyng/sys/cpu.h
	src/main/include/cyng/sys/process.h
	src/main/include/cyng/sys/mac.h
	src/main/include/cyng/sys/info.h
	src/main/include/cyng/sys/fsys.h
)

# define the sys lib
set (sys_lib
  ${sys_cpp}
  ${sys_h}
)
