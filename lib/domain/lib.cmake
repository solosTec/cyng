# 
#	reset 
#
set (domain_lib)

set (domain_cpp
    lib/domain/src/fs_domain.cpp
    lib/domain/src/log_domain.cpp
    lib/domain/src/asio_domain.cpp
    lib/domain/src/store_domain.cpp
)
    
set (domain_h
    src/main/include/cyng/compatibility/io_service.h
    src/main/include/cyng/vm/domain/log_domain.h
    src/main/include/cyng/vm/domain/fs_domain.h
    src/main/include/cyng/vm/domain/asio_domain.h
    src/main/include/cyng/vm/domain/store_domain.h
)


# define the domain lib
set (domain_lib
  ${domain_cpp}
  ${domain_h}
)
