cmake_minimum_required (VERSION 3.14)
project(PARSE)

message(STATUS "")                                                           
message(STATUS "  ,-.----.                                             ")
message(STATUS "  \\    /  \\              __  ,-.                       ")
message(STATUS "  |   :    |           ,' ,'/ /|  .--.--.              ")
message(STATUS "  |   | .\\ :  ,--.--.  '  | |' | /  /    '     ,---.   ")
message(STATUS "  .   : |: | /       \\ |  |   ,'|  :  /`./    /     \\  ")
message(STATUS "  |   |  \\ :.--.  .-. |'  :  /  |  :  ;_     /    /  | ")
message(STATUS "  |   : .  | \\__\\/: . .|  | '    \\  \\    `. .    ' / | ")
message(STATUS "  :     |`-' ,\" .--.; |;  : |     `----.   \\'   ;   /| ")
message(STATUS "  :   : :   /  /  ,.  ||  , ;    /  /`--'  /'   |  / | ")
message(STATUS "  |   | :  ;  :   .'   \\---'    '--'.     / |   :    | ")
message(STATUS "  `---'.|  |  ,     .-./          `--'---'   \\   \\  /  ")
message(STATUS "    `---`   `--`---'                          `----'   ")
message(STATUS "                                                       ")
message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR   : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR   : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")



#
# add library: cyng_parse
#
include (lib.cmake)
add_library(cyng_parse ${GLOBAL_LIBRARY_TYPE} ${parse_lib})
add_library(cyng::parse ALIAS "cyng_parse")

target_include_directories(cyng_parse
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_parse PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug parse library
#
target_compile_definitions(cyng_parse 
    PRIVATE 
        $<$<CONFIG:Debug>:__DEBUG_PARSE>
)

#
#   There is a dependency to the io library
#   dependencies: cyng_io
#
target_link_libraries(cyng_parse 
    PRIVATE 
        "$<$<PLATFORM_ID:Linux>:cyng_io>"
        Boost::boost 
)
