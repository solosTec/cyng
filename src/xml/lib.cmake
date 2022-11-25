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
    ${PROJECT_SOURCE_DIR}/../../include/cyng/xml/xml.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/xml/reader.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/xml/node.h
)


# define the docscript lib
set (xml_lib
  ${xml_cpp}
  ${xml_h}
)

