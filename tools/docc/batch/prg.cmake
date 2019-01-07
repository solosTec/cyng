# 
#	reset 
#
set (batch)

set (batch_cpp
	tools/docc/batch/src/main.cpp
	tools/docc/batch/src/batch.cpp
	tools/docc/src/driver.cpp
	tools/docc/src/reader.cpp
)
    
set (batch_h
	tools/docc/batch/src/batch.h
	tools/docc/src/driver.h
	tools/docc/src/reader.h
)

# define the batch program
set (batch
  ${batch_cpp}
  ${batch_h}
)
