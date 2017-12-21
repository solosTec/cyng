# 
#	reset 
#
set (demo)

set (demo_cpp
	test/demo/main.cpp
)
    
set (demo_h
    src/main/include/cyng/cyng.h
)

# define the demo program
set (demo
  ${demo_cpp}
  ${demo_h}
)
