cmake_minimum_required (VERSION 3.14)
project(VM)


message(STATUS "")                                                                                                                                                           
message(STATUS "                        ____       ")
message(STATUS "                      ,'  , `.     ")
message(STATUS "         ,---.     ,-+-,.' _ |     ")
message(STATUS "        /__./|  ,-+-. ;   , ||     ")
message(STATUS "   ,---.;  ; | ,--.'|'   |  ;|     ")
message(STATUS "  /___/ \\  | ||   |  ,', |  ':    ")
message(STATUS "  \\   ;  \\ ' ||   | /  | |  ||   ")
message(STATUS "   \\   \\  \\: |'   | :  | :  |,  ")
message(STATUS "    ;   \\  ' .;   . |  ; |--'     ")
message(STATUS "     \\   \\   '|   : |  | ,       ")
message(STATUS "      \\   `  ;|   : '  |/         ")
message(STATUS "       :   \\ |;   | |`-'          ")
message(STATUS "        '---\" |   ;/              ")
message(STATUS "              '---'                ")
message(STATUS "                                   ")
message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR      : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR      : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")



#
# add library: cyng_vm
#
include (lib.cmake)
add_library(cyng_vm ${GLOBAL_LIBRARY_TYPE} ${vm_lib})
add_library(cyng::vm ALIAS "cyng_vm")

target_include_directories(cyng_vm
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${pugixml_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_vm 
    PRIVATE 
        cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug also class object
#
target_compile_definitions(cyng_vm 
    PRIVATE 
        $<$<CONFIG:Debug>:__DEBUG_OBJECT>
        $<$<CONFIG:Debug>:__DEBUG_VM>
)
