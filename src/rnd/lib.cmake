# 
#	reset 
#
set (rnd_lib)

set (rnd_cpp
    src/rnd.cpp
    src/statistics.cpp
)
    
set (rnd_h
    ${CMAKE_SOURCE_DIR}/include/cyng/rnd/rnd.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/rnd/statistics.h
)

# define the docscript lib
set (rnd_lib
  ${rnd_cpp}
  ${rnd_h}
)

