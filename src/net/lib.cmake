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
    src/http_client.cpp
    src/http_client_factory.cpp
    src/http_client_proxy.cpp
)
    
set (net_h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/net.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/client.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/client_factory.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/client_proxy.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/server.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/server_factory.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/server_proxy.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/resolver.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/http_client.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/http_client_factory.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/net/http_client_proxy.h
)


# define the docscript lib
set (net_lib
  ${net_cpp}
  ${net_h}
)

