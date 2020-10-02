# 
#	reset 
#
set (json_lib)

set (json_cpp
  	lib/json/src/json.cpp
  	lib/json/src/json_parser.hpp
  	lib/json/src/json_parser.cpp
  	lib/json/src/json_inc_parser.cpp
)
    
set (json_h
    src/main/include/cyng/json.h
    src/main/include/cyng/json/json_parser.h
    src/main/include/cyng/json/json_inc_parser.h
)

set (json_imp
    src/main/include/cyng/json/json_token.h
    src/main/include/cyng/json/json_symbol.h
    src/main/include/cyng/json/json_tokenizer.h
    src/main/include/cyng/json/json_sanitizer.h
  	lib/json/src/json_token.cpp
  	lib/json/src/json_symbol.cpp
  	lib/json/src/json_tokenizer.cpp
  	lib/json/src/json_sanitizer.cpp
)

source_group("details" FILES ${json_imp})


# define the json lib
set (json_lib
  ${json_cpp}
  ${json_h}
  ${json_imp}
)
