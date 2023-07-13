# 
#	reset 
#
# 
#	reset 
#
set (asm_app)

set (asm_cpp
    src/main.cpp
    src/reader.cpp
    src/code.cpp
    src/position.cpp
    src/tokenizer.cpp
    src/token.cpp
    src/parser.cpp
)
    
set (asm_h
    include/reader.h
    include/code.h
    include/position.h
    include/tokenizer.h
    include/token.h
    include/parser.h
)

set (asm_app
  ${asm_cpp}
  ${asm_h}
)
