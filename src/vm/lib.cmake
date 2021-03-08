# 
#	reset 
#
set (vm_lib)

set (vm_cpp
    src/vm/vm.cpp
    src/vm/mesh.cpp
    src/vm/proxy.cpp
    src/vm/memory.cpp
    src/vm/stack.cpp
    src/vm/context.cpp
    src/vm/generator.cpp
    src/vm/linearize.cpp
)
    
set (vm_h
    include/cyng/vm/context.h
    include/cyng/vm/memory.h
    include/cyng/vm/mesh.h
    include/cyng/vm/proxy.h
    include/cyng/vm/stack.h
    include/cyng/vm/vm.h
    include/cyng/vm/vm_fwd.h
    include/cyng/vm/generator.hpp
    include/cyng/vm/linearize.hpp
)

# define the docscript lib
set (vm_lib
  ${vm_cpp}
  ${vm_h}
)

