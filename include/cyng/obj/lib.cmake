# 
#	reset 
#
set (sys_lib)

set (sys_cpp
    src/sys/host.cpp
    src/sys/process.cpp
    src/sys/memory.cpp
    src/sys/mac.cpp
    src/sys/port.cpp
    src/sys/locale.cpp
    src/sys/filesystem.cpp
    src/sys/ntp.cpp
)
    
set (sys_h
    include/cyng/sys/host.h
    include/cyng/sys/process.h
    include/cyng/sys/memory.h
    include/cyng/sys/mac.h
    include/cyng/sys/port.h
    include/cyng/sys/locale.h
    include/cyng/sys/filesystem.h
    include/cyng/sys/ntp.h
    include/cyng/sys/linux.hpp
    include/cyng/sys/windows.hpp
)

# define the docscript lib
set (sys_lib
  ${sys_cpp}
  ${sys_h}
)

