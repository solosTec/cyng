# 
#	reset 
#
set (vm_lib)

set (vm_cpp
    src/vm/vm.cpp
    src/vm/mesh.cpp
    src/vm/memory.cpp
    src/vm/stack.cpp
    src/vm/context.cpp
    src/vm/library.cpp
)
    
set (vm_h
    include/cyng/vm/context.h
    include/cyng/vm/library.h
    include/cyng/vm/memory.h
    include/cyng/vm/memory_interface.h
    include/cyng/vm/mesh.h
    include/cyng/vm/stack.h
    include/cyng/vm/stack_interface.h
    include/cyng/vm/vm.h
)

# define the docscript lib
set (vm_lib
  ${vm_cpp}
  ${vm_h}
)

