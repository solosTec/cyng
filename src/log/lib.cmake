# 
#	reset 
#
set (log_lib)

set (log_cpp
    src/log/log.cpp
    src/log/record.cpp
    src/log/logger.cpp
)
    
set (log_h
    include/cyng/log/log.h
    include/cyng/log/record.h
    include/cyng/log/logger.h
)

set (log_appender
    include/cyng/log/appender.h
    include/cyng/log/appender/console.h
    include/cyng/log/appender/event_log.h
    include/cyng/log/appender/rolling_file.h
    include/cyng/log/appender/sys_log.h
    src/log/appender/appender.cpp
    src/log/appender/console.cpp
    src/log/appender/rolling_file.cpp
    src/log/appender/event_log.cpp
    src/log/appender/rolling_file.cpp
    src/log/appender/sys_log.cpp
)

source_group("appender" FILES ${log_appender})


# define the docscript lib
set (log_lib
  ${log_cpp}
  ${log_h}
  ${log_appender}
)

