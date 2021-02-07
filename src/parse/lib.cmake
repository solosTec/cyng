# 
#	reset 
#
set (parse_lib)

set (parse_cpp
    src/parse/mac.cpp
    src/parse/hex.cpp
    src/parse/utf-8.cpp
)
    
set (parse_h
    include/cyng/parse/mac.h
    include/cyng/parse/hex.h
    include/cyng/parse/utf-8.h
)

set (parse_json
    include/cyng/parse/json/json_parser.h
    include/cyng/parse/json/json_symbol.h
#    include/cyng/parse/json/json_token.h
    include/cyng/parse/json/json_tokenizer.h
    src/parse/json/json_parser.cpp
    src/parse/json/json_symbol.cpp
#    src/parse/json/json_token.cpp
    src/parse/json/json_tokenizer.cpp
)

source_group("json" FILES ${parse_json})

# define the docscript lib
set (parse_lib
  ${parse_cpp}
  ${parse_h}
  ${parse_json}
)

