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
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/context.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/memory.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/mesh.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/proxy.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/stack.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/vm.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/vm_fwd.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/generator.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/vm/linearize.hpp
)

# define the docscript lib
set (vm_lib
  ${vm_cpp}
  ${vm_h}
)

