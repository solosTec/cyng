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
    src/clock.cpp
)
    
set (sys_h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/host.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/process.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/memory.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/mac.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/port.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/locale.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/filesystem.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/dns.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/ntp.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/info.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/cpu.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/net.h
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/clock.h
)

if(WIN32)
    set(sys_platform
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/windows.h
    src/windows.cpp
    )
else()
    set(sys_platform
    ${CMAKE_SOURCE_DIR}/include/cyng/sys/linux.hpp
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

