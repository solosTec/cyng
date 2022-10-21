cmake_minimum_required (VERSION 3.14)
project(TASK)


message(STATUS "")                                                                                                                                      
message(STATUS "      ___                                  ,-.        ")
message(STATUS "    ,--.'|_                            ,--/ /|        ")
message(STATUS "    |  | :,'                         ,--. :/ |        ")
message(STATUS "    :  : ' :               .--.--.   :  : ' /         ")
message(STATUS "  .;__,'  /    ,--.--.    /  /    '  |  '  /          ")
message(STATUS "  |  |   |    /       \\  |  :  /`./  '  |  :         ")
message(STATUS "  :__,'| :   .--.  .-. | |  :  ;_    |  |   \\        ")
message(STATUS "    '  : |__  \\__\\/: . .  \\  \\    `. '  : |. \\   ")
message(STATUS "    |  | '.'| ,\" .--.; |   `----.   \\|  | ' \\ \\   ")
message(STATUS "    ;  :    ;/  /  ,.  |  /  /`--'  /'  : |--'        ")
message(STATUS "    |  ,   /;  :   .'   \\'--'.     / ;  |,'          ")
message(STATUS "     ---`-' |  ,     .-./  `--'---'  '--'             ")
message(STATUS "             `--`---'                                 ")
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
# add library: cyng_task
#
include (lib.cmake)
add_library(cyng_task ${GLOBAL_LIBRARY_TYPE} ${task_lib})
add_library(cyng::task ALIAS "cyng_task")

target_include_directories(cyng_task
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_task PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug class object
#
target_compile_definitions(cyng_task 
    PRIVATE 
        $<$<CONFIG:Debug>:__DEBUG_TASK>
        $<$<AND:$<VERSION_LESS_EQUAL:${CMAKE_CXX_COMPILER_VERSION},9.0>,$<CXX_COMPILER_ID:GNU>>:BOOST_ASIO_HAS_STD_FUTURE>       
)

target_compile_options(cyng_task 
    PRIVATE 
        $<$<AND:$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},7.1>,$<CXX_COMPILER_ID:GNU>>:-Wno-psabi>
)
