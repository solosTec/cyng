# 
#	reset 
#
set (docc)

set (docc_cpp
	tools/docc/src/main.cpp
	tools/docc/src/driver.cpp
	tools/docc/src/reader.cpp
)
    
set (docc_h
	tools/docc/src/driver.h
	tools/docc/src/reader.h
)

# define the docc program
set (docc
  ${docc_cpp}
  ${docc_h}
)
