cmake_minimum_required (VERSION 3.14)
project(IO)


message(STATUS "")                                                           
message(STATUS "                            ,----..     ")
message(STATUS "     ,---,          ,--,   /   /   \\   ")
message(STATUS "  ,`--.' |         / .`|  /   .     :   ")
message(STATUS "  |   :  :        /' / ; .   /   ;.  \\ ")
message(STATUS "  :   |  '       /  / .'.   ;   /  ` ;  ")
message(STATUS "  |   :  |      /  / ./ ;   |  ; \\ ; | ")
message(STATUS "  '   '  ;     / ./  /  |   :  | ; | '  ")
message(STATUS "  |   |  |    /  /  /   .   |  ' ' ' :  ")
message(STATUS "  '   :  ;   /  /  /    '   ;  \\; /  | ")
message(STATUS "  |   |  '  ;  /  /      \\   \\  ',  / ")
message(STATUS "  '   :  |./__;  /        ;   :    /    ")
message(STATUS "  ;   |.' |   : /          \\   \\ .'   ")
message(STATUS "  '---'   ;   |/            `---`       ")
message(STATUS "          `---'                         ")
message(STATUS "")                                                           
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR :      ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR :      ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")
message(STATUS "** PUGIXML_INCLUDE_DIRS: ${PUGIXML_INCLUDE_DIRS}")


#
# add library: cyng_io
#
include (lib.cmake)
add_library(cyng_io ${GLOBAL_LIBRARY_TYPE} ${io_lib})
add_library(cyng::io ALIAS "cyng_io")

target_include_directories(cyng_io
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/../../include>
        $<BUILD_INTERFACE:${date_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PUGIXML_INCLUDE_DIRS}>
        "/usr/include"
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE
        "/usr/include"
)

target_compile_features(cyng_io PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug i/o library
#
target_compile_definitions(cyng_io 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_IO>
        $<$<BOOL:${CYNG_USE_FEATURE_TESTING}>:__CYNG_USE_FEATURE_TESTING>
        $<$<BOOL:${CYNG_USE_DATE_LIBRARY}>:__CYNG_USE_DATE_LIBRARY>     
        $<$<CXX_COMPILER_ID:MSVC>:_WIN32_WINNT=0x0A00>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>   
)

target_link_libraries(cyng_io 
    PRIVATE 
        Boost::boost 
        pugixml
)
