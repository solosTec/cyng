# 
#	reset 
#
set (sys_lib)

set (sys_cpp
    src/host.cpp
    src/process.cpp
    src/memory.cpp
    src/mac.cpp
    src/port.cpp
    src/locale.cpp
    src/filesystem.cpp
    src/dns.cpp
    src/ntp.cpp
    src/info.cpp
    src/cpu.cpp
    src/net.cpp
#    src/clock.cpp
)
    
set (sys_h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/host.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/process.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/memory.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/mac.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/port.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/locale.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/filesystem.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/dns.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/ntp.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/info.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/cpu.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/net.h
#    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/clock.h
)

if(WIN32)
    set(sys_platform
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/windows.h
    src/windows.cpp
    )
else()
    set(sys_platform
    ${PROJECT_SOURCE_DIR}/../../include/cyng/sys/linux.hpp
    src/linux.cpp
    )
endif()

source_group("platform" FILES ${sys_platform})


# define the docscript lib
set (sys_lib
  ${sys_cpp}
  ${sys_h}
  ${sys_platform}
)

