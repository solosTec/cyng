# 
#	reset 
#
set (scm_lib)

set (scm_cpp
 	lib/scm/src/service.cpp
 	lib/scm/src/mgr.cpp
)
    
set (scm_h
	src/main/include/cyng/scm/win_registry.h
    src/main/include/cyng/scm/service.hpp
    src/main/include/cyng/scm/mgr.h
)

# define the scm lib
set (scm_lib
  ${scm_cpp}
  ${scm_h}
)
