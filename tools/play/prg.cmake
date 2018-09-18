# 
#	reset 
#
set (play)

set (play_cpp
	tools/play/src/main.cpp
	tools/play/src/play.cpp
)
    
set (play_h
	tools/play/src/play.h
)


# define the play program
set (play
  ${play_cpp}
  ${play_h}
)
