# 
#	reset 
#
set (parse_lib)

set (parse_cpp
    src/parse/mac.cpp
    src/parse/hex.cpp
)
    
set (parse_h
    include/cyng/parse/mac.h
    include/cyng/parse/hex.h
)

# define the docscript lib
set (parse_lib
  ${parse_cpp}
  ${parse_h}
)

