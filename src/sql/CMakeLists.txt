cmake_minimum_required (VERSION 3.14)
project(SQL)


message(STATUS "")                                                           
message(STATUS "                                 ,--,       ")
message(STATUS "                              ,---.'|       ")
message(STATUS "    .--.--.       ,----..     |   | :       ")
message(STATUS "   /  /    '.    /   /   \\    :   : |      ")
message(STATUS "  |  :  /`. /   /   .     :   |   ' :       ")
message(STATUS "  ;  |  |--`   .   /   ;.  \\  ;   ; '      ")
message(STATUS "  |  :  ;_    .   ;   /  ` ;  '   | |__     ")
message(STATUS "   \\  \\    `. ;   |  ; \\ ; |  |   | :.'| ")
message(STATUS "    `----.   \\|   :  | ; | '  '   :    ;   ")
message(STATUS "    __ \\  \\  |.   |  ' ' ' :  |   |  ./   ")
message(STATUS "   /  /`--'  /'   ;  \\; /  |  ;   : ;      ")
message(STATUS "  '--'.     /  \\   \\  ',  . \\ |   ,/     ")
message(STATUS "    `--'---'    ;   :      ; |'---'         ")
message(STATUS "                 \\   \\ .'`--\"            ")
message(STATUS "                  `---`                     ")
message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")

#
# add library: cyng_sql
#
include (lib.cmake)
add_library(cyng_sql ${GLOBAL_LIBRARY_TYPE} ${sql_lib})
add_library(cyng::sql ALIAS "cyng_sql")

target_include_directories(cyng_sql
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/../../include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_sql PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug sql library
#
target_compile_definitions(cyng_sql 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_SQL>
)

target_link_libraries(cyng_sql 
    PRIVATE 
        Boost::boost 
)
