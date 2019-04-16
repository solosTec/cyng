# top level files
set (shell)

set (shell_cpp

    3party/sqlite-amalgamation-3270200/shell.c  

)

set (shell_h

    3party/sqlite-amalgamation-3270200/sqlite3.h
   
)


# define the SQLite3 shell
set (shell
  ${shell_cpp}
  ${shell_h}
)
