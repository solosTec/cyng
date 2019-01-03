# 
#	reset 
#
set (docscript_lib)

set (docscript_cpp
	lib/docscript/src/compiler.cpp  
	lib/docscript/src/generator.cpp  
	lib/docscript/src/lexer.cpp  
	lib/docscript/src/library.cpp  
	lib/docscript/src/parser.cpp  
	lib/docscript/src/parser.hpp  
	lib/docscript/src/statistics.cpp  
	lib/docscript/src/symbol.cpp  
	lib/docscript/src/token.cpp  
	lib/docscript/src/tokenizer.cpp
)
    
set (docscript_h
	src/main/include/cyng/docscript/compiler.h  
	src/main/include/cyng/docscript/docscript.h  
	src/main/include/cyng/docscript/generator.h  
	src/main/include/cyng/docscript/lexer.h  
	src/main/include/cyng/docscript/library.h  
	src/main/include/cyng/docscript/parser.h  
	src/main/include/cyng/docscript/statistics.h  
	src/main/include/cyng/docscript/symbol.h  
	src/main/include/cyng/docscript/token.h  
	src/main/include/cyng/docscript/tokenizer.h
)

set (docscript_filter
	lib/docscript/src/filter/verbatim.cpp
	lib/docscript/src/filter/cpp.cpp
	src/main/include/cyng/docscript/filter/verbatim.h
	src/main/include/cyng/docscript/filter/cpp.h
)

source_group("filter" FILES ${docscript_filter})

# define the docscript lib
set (docscript_lib
  ${docscript_cpp}
  ${docscript_h}
  ${docscript_filter}
)
