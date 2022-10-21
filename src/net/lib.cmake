# 
#	reset 
#
set (net_lib)

set (net_cpp
    src/client.cpp
    src/client_factory.cpp
    src/client_proxy.cpp
    src/server.cpp
    src/server_factory.cpp
    src/server_proxy.cpp
    src/resolver.cpp
)
    
set (net_h
    ${CMAKE_SOURCE_DIR}/include/cyng/net/client.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/net/client_factory.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/net/client_proxy.h
    ${CMAKE_SOURCE_DIR}/include/cyng/net/server.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/net/server_factory.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/net/server_proxy.h
    ${CMAKE_SOURCE_DIR}/include/cyng/net/resolver.hpp
)


# define the docscript lib
set (net_lib
  ${net_cpp}
  ${net_h}
)

