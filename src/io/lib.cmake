# 
#	reset 
#
set (io_lib)

set (io_cpp
    src/ostream.cpp
    src/io_buffer.cpp
)
    
set (io_h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/hex_dump.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/io.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/io_buffer.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/ostream.h
)

set (io_serializer
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serialize.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serialize.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/binary.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/csv.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/json.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/json_walker.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/plain.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/typed.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/xml.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/cpp.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/serializer/pretty_walker.h
    src/serialize.cpp
    src/serializer/io.cpp
    src/serializer/binary.cpp
    src/serializer/csv.cpp
    src/serializer/json.cpp
    src/serializer/json_walker.cpp
    src/serializer/plain.cpp
    src/serializer/typed.cpp
    src/serializer/xml.cpp
    src/serializer/cpp.cpp
    src/serializer/pretty_walker.cpp
)

set (io_parser
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/parser/parser.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/parser/utf-8.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/parser/stream.hpp
    src/parser/parser.cpp
    src/parser/utf-8.cpp
    src/parser/stream.cpp
)

set (io_iso
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/iso_639_1.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/io/iso_3166_1.h
    src/iso_639_1.cpp
    src/iso_3166_1.cpp
)

#set (io_xml
#    ${pugixml_SOURCE_DIR}/src/pugixml.hpp
#    ${pugixml_SOURCE_DIR}/src/pugixml.cpp
#)

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

