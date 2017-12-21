# 
#	reset 
#
set (meta_lib)

set (meta_cpp
	lib/meta/src/meta.cpp 	
)
    
set (meta_h
    src/main/include/cyng/cyng.h
    src/main/include/cyng/meta.h
    src/main/include/cyng/meta/meta.hpp
    src/main/include/cyng/meta/size.hpp
    src/main/include/cyng/meta/conversion.hpp
)

# define the meta lib
set (meta_lib
  ${meta_cpp}
  ${meta_h}
)
