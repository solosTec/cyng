# 
#	reset 
#
set (vm_lib)

set (vm_cpp
    src/vm.cpp
    src/mesh.cpp
    src/proxy.cpp
    src/memory.cpp
    src/stack.cpp
    src/context.cpp
    src/generator.cpp
    src/linearize.cpp
)
    
set (vm_h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/context.h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/memory.h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/mesh.h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/proxy.h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/stack.h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/vm.h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/vm_fwd.h
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/generator.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/vm/linearize.hpp
)

# define the docscript lib
set (vm_lib
  ${vm_cpp}
  ${vm_h}
)

