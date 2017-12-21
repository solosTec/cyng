# 
#	reset 
#
set (json_lib)

set (json_cpp
  	lib/json/src/json.cpp
  	lib/json/src/json_parser.hpp
  	lib/json/src/json_parser.cpp
)
    
set (json_h
    src/main/include/cyng/json.h
    src/main/include/cyng/json/json_parser.h
)

# define the json lib
set (json_lib
  ${json_cpp}
  ${json_h}
)
