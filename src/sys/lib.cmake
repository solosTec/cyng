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
    src/sys/dns.cpp
    src/sys/ntp.cpp
    src/sys/info.cpp
    src/sys/cpu.cpp
    src/sys/net.cpp
)
    
set (sys_h
    include/cyng/sys/host.h
    include/cyng/sys/process.h
    include/cyng/sys/memory.h
    include/cyng/sys/mac.h
    include/cyng/sys/port.h
    include/cyng/sys/locale.h
    include/cyng/sys/filesystem.h
    include/cyng/sys/dns.h
    include/cyng/sys/ntp.h
    include/cyng/sys/info.h
    include/cyng/sys/cpu.h
    include/cyng/sys/net.h
)

if(WIN32)
    set(sys_platform
    include/cyng/sys/windows.h
    src/sys/windows.cpp
    )
else()
    set(sys_platform
    include/cyng/sys/linux.h
    src/sys/linux.cpp
    )
endif()

source_group("platform" FILES ${sys_platform})


# define the docscript lib
set (sys_lib
  ${sys_cpp}
  ${sys_h}
  ${sys_platform}
)

