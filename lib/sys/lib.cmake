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
  	lib/sys/src/ntp.cpp
  	lib/sys/src/dns.cpp
  	lib/sys/src/ip.cpp
)
    
set (sys_h
	src/main/include/cyng/sys/memory.h
	src/main/include/cyng/sys/cpu.h
	src/main/include/cyng/sys/process.h
	src/main/include/cyng/sys/mac.h
	src/main/include/cyng/sys/info.h
	src/main/include/cyng/sys/fsys.h
	src/main/include/cyng/sys/ntp.h
	src/main/include/cyng/sys/dns.h
	src/main/include/cyng/sys/ip.h
)

# define the sys lib
set (sys_lib
  ${sys_cpp}
  ${sys_h}
)

if (WIN32)
	list(APPEND sys_lib src/main/include/cyng/scm/win_registry.h)
	list(APPEND sys_lib lib/scm/src/win_registry.cpp)
endif(WIN32)