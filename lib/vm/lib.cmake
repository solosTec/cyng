# 
#	reset 
#
set (vm_lib)

set (vm_cpp
	lib/vm/src/vm.cpp
	lib/vm/src/controller.cpp
	lib/vm/src/manip.cpp
	lib/vm/src/memory.cpp
	lib/vm/src/stack.cpp
	lib/vm/src/librarian.cpp
	lib/vm/src/context.cpp
	lib/vm/src/procedure.cpp
	lib/vm/src/generator.cpp
)
    
set (vm_h
    src/main/include/cyng/compatibility/io_service.h
    src/main/include/cyng/intrinsics/op.h
    src/main/include/cyng/vm/vm.h
    src/main/include/cyng/vm/controller.h
    src/main/include/cyng/vm/manip.h
    src/main/include/cyng/vm/memory.h
    src/main/include/cyng/vm/stack.h
    src/main/include/cyng/vm/librarian.h
    src/main/include/cyng/vm/context.h
    src/main/include/cyng/vm/procedure.h
    src/main/include/cyng/vm/generator.h
)


# define the vm lib
set (vm_lib
  ${vm_cpp}
  ${vm_h}
)
