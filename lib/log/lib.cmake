# 
#	reset 
#
set (log_lib)

set (log_cpp
 	lib/log/src/severity.cpp
)
    
set (log_h
    src/main/include/cyng/cyng.h
	src/main/include/cyng/compatibility/legacy_mode.hpp
    src/main/include/cyng/log.h
    src/main/include/cyng/log/severity.h
    src/main/include/cyng/log/entry.hpp
    src/main/include/cyng/log/logger.hpp
)

# define the log lib
set (log_lib
  ${log_cpp}
  ${log_h}
)
