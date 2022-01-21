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
    include/cyng/io/serializer/json_walker.h
    include/cyng/io/serializer/plain.hpp
    include/cyng/io/serializer/typed.hpp
    include/cyng/io/serializer/xml.hpp
    include/cyng/io/serializer/cpp.hpp
    include/cyng/io/serializer/pretty_walker.h
    src/io/serialize.cpp
    src/io/serializer/io.cpp
    src/io/serializer/binary.cpp
    src/io/serializer/csv.cpp
    src/io/serializer/json.cpp
    src/io/serializer/json_walker.cpp
    src/io/serializer/plain.cpp
    src/io/serializer/typed.cpp
    src/io/serializer/xml.cpp
    src/io/serializer/cpp.cpp
    src/io/serializer/pretty_walker.cpp
)

set (io_parser
    include/cyng/io/parser/parser.h
    include/cyng/io/parser/utf-8.hpp
    include/cyng/io/parser/stream.hpp
    src/io/parser/parser.cpp
    src/io/parser/utf-8.cpp
    src/io/parser/stream.cpp
)

set (io_iso
    include/cyng/io/iso_639_1.h
    include/cyng/io/iso_3166_1.h
    src/io/iso_639_1.cpp
    src/io/iso_3166_1.cpp
)

set (io_xml
    ${pugixml_SOURCE_DIR}/src/pugixml.hpp
    ${pugixml_SOURCE_DIR}/src/pugixml.cpp
)

source_group("serializer" FILES ${io_serializer})
source_group("parser" FILES ${io_parser})
source_group("iso" FILES ${io_iso})
source_group("xml" FILES ${io_xml})


# define the docscript lib
set (io_lib
  ${io_cpp}
  ${io_h}
  ${io_serializer}
  ${io_parser}
  ${io_iso}
  ${io_xml}
)

