# 
#	reset 
#
set (rnd_lib)

set (rnd_cpp
    src/rnd/rnd.cpp
    src/rnd/statistics.cpp
)
    
set (rnd_h
    include/cyng/rnd/rnd.hpp
    include/cyng/rnd/statistics.h
)

# define the docscript lib
set (rnd_lib
  ${rnd_cpp}
  ${rnd_h}
)

