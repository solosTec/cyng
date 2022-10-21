cmake_minimum_required (VERSION 3.14)
project(NETWORK)


message(STATUS "")                                                           
message(STATUS "                            ___        ")
message(STATUS "                          ,--.'|_      ")
message(STATUS "        ,---,             |  | :,'     ")
message(STATUS "    ,-+-. /  |            :  : ' :     ")
message(STATUS "   ,--.'|'   |   ,---.  .;__,'  /      ")
message(STATUS "  |   |  ,\"' |  /     \\ |  |   |     ")
message(STATUS "  |   | /  | | /    /  |:__,'| :       ")
message(STATUS "  |   | |  | |.    ' / |  '  : |__     ")
message(STATUS "  |   | |  |/ '   ;   /|  |  | '.'|    ")
message(STATUS "  |   | |--'  '   |  / |  ;  :    ;    ")
message(STATUS "  |   |/      |   :    |  |  ,   /     ")
message(STATUS "  '---'        \\   \\  /    ---`-'    ")
message(STATUS "                `----'                 ")
message(STATUS "")                                                           
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")


include (lib.cmake)

add_library(cyng_net ${GLOBAL_LIBRARY_TYPE} ${net_lib})
add_library(cyng::net ALIAS "cyng_net")

target_compile_features(cyng_net PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

target_compile_definitions(cyng_net
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_NET>
)

#
# -Wno-psabi for gcc > 7.1
#
target_compile_options(cyng_net 
    PRIVATE 
        $<$<AND:$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},7.1>,$<CXX_COMPILER_ID:GNU>>:-Wno-psabi>
)

target_include_directories(cyng_net
    PRIVATE
#        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
