# 
#	reset 
#
set (io_lib)

set (io_cpp
 	lib/io/src/plain.cpp
  	lib/io/src/binary.cpp
 	lib/io/src/formatted.cpp
 	lib/io/src/json.cpp
 	lib/io/src/io_chrono.cpp
 	lib/io/src/serialize.cpp
 	lib/io/src/deserializer.cpp
 	lib/io/src/parser.cpp
 	lib/io/src/object_reader.cpp
)
    
set (io_h
    src/main/include/cyng/cyng.h
    src/main/include/cyng/io.h
    src/main/include/cyng/io/hex_dump.hpp
    src/main/include/cyng/io/io_chrono.hpp
    src/main/include/cyng/io/serializer/plain.hpp
    src/main/include/cyng/io/serializer/binary.hpp
    src/main/include/cyng/io/serializer/formatted.hpp
    src/main/include/cyng/io/serializer/json.hpp
    src/main/include/cyng/io/serializer/serialize.hpp
    src/main/include/cyng/io/serializer.h
    src/main/include/cyng/io/parser/parser.h
    src/main/include/cyng/io/parser/deserializer.h
    src/main/include/cyng/io/object_reader.hpp
)

# define the io lib
set (io_lib
  ${io_cpp}
  ${io_h}
)
