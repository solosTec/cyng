# 
#	reset 
#
set (scm_lib)

set (scm_cpp
 	src/scm/service.cpp
 	src/scm/mgr.cpp
 	src/scm/win_registry.cpp
)
    
set (scm_h
	include/cyng/scm/win_registry.h
    include/cyng/scm/service.hpp
    include/cyng/scm/mgr.h
)

# define the scm lib
set (scm_lib
  ${scm_cpp}
  ${scm_h}
)
