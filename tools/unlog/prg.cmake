# 
#	reset 
#
set (unlog)

set (unlog_cpp
	tools/unlog/src/main.cpp
#	tools/unlog/src/driver.cpp
#	tools/unlog/src/reader.cpp
)
    
set (unlog_filter
	tools/unlog/src/filters/hexdump.h
	tools/unlog/src/filters/hexdump.cpp
	tools/unlog/src/filters/hexdump_cpp.h
	tools/unlog/src/filters/hexdump_cpp.cpp
)

source_group("filter" FILES ${unlog_filter})

# define the unlog program
set (unlog
  ${unlog_cpp}
  ${unlog_filter}
)
