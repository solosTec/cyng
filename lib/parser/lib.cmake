# 
#	reset 
#
set (parser_lib)

set (parser_cpp
 	lib/parser/src/numeric_parser.cpp
 	lib/parser/src/numeric_parser.hpp
 	lib/parser/src/object_parser.cpp
 	lib/parser/src/object_parser.hpp
 	lib/parser/src/utf8_parser.cpp
 	lib/parser/src/utf8_parser.hpp
 	lib/parser/src/token_parser.cpp
 	lib/parser/src/token_parser.hpp
 	lib/parser/src/chrono_parser.cpp
 	lib/parser/src/chrono_parser.hpp
 	lib/parser/src/buffer_parser.cpp
 	lib/parser/src/buffer_parser.hpp
 	lib/parser/src/bom_parser.cpp
)
    
set (parser_h
    src/main/include/cyng/parser/spirit_support.hpp
    src/main/include/cyng/parser/numeric_parser.h
    src/main/include/cyng/parser/object_parser.h
    src/main/include/cyng/parser/utf8_parser.h
    src/main/include/cyng/parser/token_parser.h
    src/main/include/cyng/parser/chrono_parser.h
    src/main/include/cyng/parser/buffer_parser.h
    src/main/include/cyng/parser/bom_parser.h
)

# define the parser lib
set (parser_lib
  ${parser_cpp}
  ${parser_h}
)
