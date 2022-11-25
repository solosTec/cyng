# 
#	reset 
#
set (scm_lib)

set (scm_cpp
 	src/service.cpp
 	src/mgr.cpp
 	src/win_registry.cpp
)
    
set (scm_h
	${PROJECT_SOURCE_DIR}/../../include/cyng/scm/win_registry.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/scm/service.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/scm/mgr.h
)

# define the scm lib
set (scm_lib
  ${scm_cpp}
  ${scm_h}
)
