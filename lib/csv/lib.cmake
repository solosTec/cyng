# 
#	reset 
#
set (csv_lib)

set (csv_cpp
  	lib/csv/src/csv.cpp
  	lib/csv/src/csv_parser.hpp
  	lib/csv/src/csv_parser.cpp
)
    
set (csv_h
    src/main/include/cyng/csv.h
    src/main/include/cyng/csv/csv_parser.h
)

# define the csv lib
set (csv_lib
  ${csv_cpp}
  ${csv_h}
)
