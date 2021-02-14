# 
#	reset 
#
set (log_lib)

set (log_cpp
    src/log/log.cpp
    src/log/record.cpp
    src/log/logger.cpp
    src/log/conv.cpp
)
    
set (log_h
    include/cyng/log/log.h
    include/cyng/log/record.h
    include/cyng/log/logger.h
    include/cyng/log/conv.h
)

set (log_appender
    include/cyng/log/appender.h
    include/cyng/log/appender/console.h
    include/cyng/log/appender/rolling_file.h
    
    src/log/appender/appender.cpp
    src/log/appender/console.cpp
    src/log/appender/rolling_file.cpp   
    src/log/appender/rolling_file.cpp
    
)

if (UNIX)
    list(APPEND log_appender "src/log/appender/sys_log.cpp")
    list(APPEND log_appender "include/cyng/log/appender/sys_log.h")
endif()

if (WIN32)
    list(APPEND log_appender "src/log/appender/event_log.cpp")
    list(APPEND log_appender "include/cyng/log/appender/event_log.h")
endif()


source_group("appender" FILES ${log_appender})


# define the docscript lib
set (log_lib
  ${log_cpp}
  ${log_h}
  ${log_appender}
)

