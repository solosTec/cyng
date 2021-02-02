# 
#	reset 
#
set (io_lib)

set (io_cpp
    src/io/ostream.cpp
    src/io/io_buffer.cpp
)
    
set (io_h
    include/cyng/io/hex_dump.hpp
    include/cyng/io/io.h
    include/cyng/io/io_buffer.h
    include/cyng/io/ostream.h
)

set (io_serializer
    include/cyng/io/serialize.h
    include/cyng/io/serialize.hpp
    include/cyng/io/serializer/binary.hpp
    include/cyng/io/serializer/csv.hpp
    include/cyng/io/serializer/json.hpp
    include/cyng/io/serializer/plain.hpp
    include/cyng/io/serializer/typed.hpp
    src/io/serialize.cpp
    src/io/serializer/binary.cpp
    src/io/serializer/csv.cpp
    src/io/serializer/json.cpp
    src/io/serializer/plain.cpp
    src/io/serializer/typed.cpp
)

set (io_parser
    include/cyng/io/parser/parser.h
    src/io/parser/parser.cpp
)

source_group("serializer" FILES ${io_serializer})
source_group("parser" FILES ${io_parser})


# define the docscript lib
set (io_lib
  ${io_cpp}
  ${io_h}
  ${io_serializer}
  ${io_parser}
)

