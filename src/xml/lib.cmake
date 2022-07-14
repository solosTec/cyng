# 
#	reset 
#
set (xml_lib)

set (xml_cpp
    src/xml.cpp
    src/reader.cpp
    src/node.cpp
)
    
set (xml_h
    ${CMAKE_SOURCE_DIR}/include/cyng/xml/xml.h
    ${CMAKE_SOURCE_DIR}/include/cyng/xml/reader.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/xml/node.h
)


# define the docscript lib
set (xml_lib
  ${xml_cpp}
  ${xml_h}
)

