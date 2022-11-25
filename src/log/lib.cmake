# 
#	reset 
#
set (log_lib)

set (log_cpp
    src/log.cpp
    src/record.cpp
    src/logger.cpp
    src/conv.cpp
)
    
set (log_h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/log/log.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/log/record.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/log/logger.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/log/conv.h
)

set (log_appender
    ${PROJECT_SOURCE_DIR}/../../include/cyng/log/appender.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/log/appender/console.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/log/appender/rolling_file.h
    
    src/appender/appender.cpp
    src/appender/console.cpp
    src/appender/rolling_file.cpp   
    src/appender/rolling_file.cpp
    
)

if (UNIX)
    list(APPEND log_appender "${PROJECT_SOURCE_DIR}/../../include/cyng/log/appender/sys_log.h")
    list(APPEND log_appender "src/appender/sys_log.cpp")
endif()

if (WITH_SYSTEMD)
    list(APPEND log_appender "${PROJECT_SOURCE_DIR}/../../include/cyng/log/appender/journald.h")
    list(APPEND log_appender "src/appender/journald.cpp")
endif()

if (WIN32)
    list(APPEND log_appender "src/appender/event_log.cpp")
    list(APPEND log_appender "${PROJECT_SOURCE_DIR}/../../include/cyng/log/appender/event_log.h")
endif()


source_group("appender" FILES ${log_appender})


# define the docscript lib
set (log_lib
  ${log_cpp}
  ${log_h}
  ${log_appender}
)

