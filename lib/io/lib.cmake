# 
#	reset 
#
set (io_lib)

set (io_cpp
 	lib/io/src/plain.cpp
  	lib/io/src/binary.cpp
 	lib/io/src/typed.cpp
 	lib/io/src/json.cpp
 	lib/io/src/csv.cpp
 	lib/io/src/io_chrono.cpp
 	lib/io/src/io_buffer.cpp
 	lib/io/src/serialize.cpp
 	lib/io/src/deserializer.cpp
 	lib/io/src/parser.cpp
 	lib/io/src/object_reader.cpp
 	lib/io/src/swap.cpp
 	lib/io/src/iso_3166_1.cpp
 	lib/io/src/iso_639_1.cpp
 	lib/io/src/bom.cpp
)
    
set (io_h
    src/main/include/cyng/cyng.h
    src/main/include/cyng/io.h
    src/main/include/cyng/io/hex_dump.hpp
    src/main/include/cyng/io/io_chrono.hpp
    src/main/include/cyng/io/io_buffer.h
    src/main/include/cyng/io/io_bytes.hpp
    src/main/include/cyng/io/serializer/plain.hpp
    src/main/include/cyng/io/serializer/binary.hpp
    src/main/include/cyng/io/serializer/typed.hpp
    src/main/include/cyng/io/serializer/json.hpp
    src/main/include/cyng/io/serializer/csv.hpp
    src/main/include/cyng/io/serializer/serialize.hpp
    src/main/include/cyng/io/serializer.h
    src/main/include/cyng/io/parser/parser.h
    src/main/include/cyng/io/parser/deserializer.h
    src/main/include/cyng/io/object_reader.hpp
    src/main/include/cyng/io/swap.h
    src/main/include/cyng/io/iso_3166_1.h
    src/main/include/cyng/io/iso_639_1.h
    src/main/include/cyng/io/bom.h
)


# define the io lib
set (io_lib
  ${io_cpp}
  ${io_h}
)
