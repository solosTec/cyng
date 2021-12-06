# 
#	reset 
#
set (xml_lib)

set (xml_cpp
    src/xml/xml.cpp
    src/xml/reader.cpp
    src/xml/node.cpp
)
    
set (xml_h
    include/cyng/xml/xml.h
    include/cyng/xml/reader.hpp
    include/cyng/xml/node.h
)


# define the docscript lib
set (xml_lib
  ${xml_cpp}
  ${xml_h}
)

