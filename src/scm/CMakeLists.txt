cmake_minimum_required (VERSION 3.14)
project(SCM)

message(STATUS "")                                                           
message(STATUS "                                   ____         ")
message(STATUS "    .--.--.     ,----..          ,'  , `.       ")
message(STATUS "   /  /    '.  /   /   \\      ,-+-,.' _ |      ")
message(STATUS "  |  :  /`. / |   :     :  ,-+-. ;   , ||       ")
message(STATUS "  ;  |  |--`  .   |  ;. / ,--.'|'   |  ;|       ")
message(STATUS "  |  :  ;_    .   ; /--` |   |  ,', |  ':       ")
message(STATUS "   \\  \\    `. ;   | ;    |   | /  | |  ||     ")
message(STATUS "    `----.   \\|   : |    '   | :  | :  |,      ")
message(STATUS "    __ \\  \\  |.   | '___ ;   . |  ; |--'      ")
message(STATUS "   /  /`--'  /'   ; : .'||   : |  | ,           ")
message(STATUS "  '--'.     / '   | '/  :|   : '  |/            ")
message(STATUS "    `--'---'  |   :    / ;   | |`-'             ")
message(STATUS "               \\   \\ .'  |   ;/               ")
message(STATUS "                `---`    '---'                  ")
message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR     : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR     : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")


include (lib.cmake)
add_library(cyng_scm ${GLOBAL_LIBRARY_TYPE} ${scm_lib})
add_library(cyng::scm ALIAS "cyng_scm")

target_include_directories(cyng_scm
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/../../include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_scm PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug SCM library
#
target_compile_definitions(cyng_scm 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_SCM>
)

target_link_libraries(cyng_scm 
    PRIVATE 
        Boost::boost 
)

