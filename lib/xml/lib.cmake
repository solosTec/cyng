# 
#	reset 
#
set (xml_lib)

set (xml_cpp
    lib/xml/pugixml/src/pugixml.cpp
  	lib/xml/src/xml.cpp
  	lib/xml/src/serializer.cpp
  	lib/xml/src/reader.cpp
)
    
set (xml_h
	lib/xml/pugixml/src/pugixml.hpp
    src/main/include/cyng/xml.h
    src/main/include/cyng/xml/serializer.hpp
    src/main/include/cyng/xml/reader.hpp
)

# define the xml lib
set (xml_lib
  ${xml_cpp}
  ${xml_h}
)
