# 
#	reset 
#
set (asm_lib)

set (asm_cpp
    src/reader.cpp
    src/writer.cpp
    src/code.cpp
    src/position.cpp
    src/tokenizer.cpp
    src/token.cpp
    src/parser.cpp
)
    
set (asm_h
    ${CMAKE_SOURCE_DIR}/include/cyng/asm/reader.h
    ${CMAKE_SOURCE_DIR}/include/cyng/asm/writer.h
    ${CMAKE_SOURCE_DIR}/include/cyng/asm/code.h
    ${CMAKE_SOURCE_DIR}/include/cyng/asm/position.h
    ${CMAKE_SOURCE_DIR}/include/cyng/asm/tokenizer.h
    ${CMAKE_SOURCE_DIR}/include/cyng/asm/token.h
    ${CMAKE_SOURCE_DIR}/include/cyng/asm/parser.h
)


# define the asm lib
set (asm_lib
  ${asm_cpp}
  ${asm_h}
)

