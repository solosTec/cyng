# 
#	reset 
#
set (compatibility_lib)

set (compatibility_cpp
	lib/compatibility/src/compatibility.cpp 	
)
    
set (compatibility_h
    src/main/include/cyng/compatibility.h
    src/main/include/cyng/compatibility/async.h
    src/main/include/cyng/compatibility/file_system.hpp
    src/main/include/cyng/compatibility/general.h
    src/main/include/cyng/compatibility/io_service.h
    src/main/include/cyng/compatibility/legacy_mode.hpp
    src/main/include/cyng/compatibility/optional.hpp
)

# define the meta lib
set (compatibility_lib
  ${compatibility_cpp}
  ${compatibility_h}
)
