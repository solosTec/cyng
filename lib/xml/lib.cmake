# 
#	reset 
#
set (xml_lib)

set (xml_cpp
	${PROJECT_SOURCE_DIR}/3party/pugixml-181/pugixml.cpp
  	lib/xml/src/xml.cpp
  	lib/xml/src/serializer.cpp
)
    
set (xml_h
	${PUGIXML_INCLUDE_DIR}/pugixml.hpp
    src/main/include/cyng/xml.h
    src/main/include/cyng/xml/serializer.hpp
)

# define the xml lib
set (xml_lib
  ${xml_cpp}
  ${xml_h}
)
