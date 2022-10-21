cmake_minimum_required (VERSION 3.14)
project(XML)


message(STATUS "")                                                           
message(STATUS "                                   ,--,       ")
message(STATUS "                          ____  ,---.'|       ")
message(STATUS "   ,--,     ,--,        ,'  , `.|   | :       ")
message(STATUS "   |'. \\   / .`|     ,-+-,.' _ |:   : |      ")
message(STATUS "   ; \\ `\\ /' / ;  ,-+-. ;   , |||   ' :     ")
message(STATUS "   `. \\  /  / .' ,--.'|'   |  ;|;   ; '      ")
message(STATUS "    \\  \\/  / ./ |   |  ,', |  ':'   | |__   ")
message(STATUS "     \\  \\.'  /  |   | /  | |  |||   | :.'|  ")
message(STATUS "      \\  ;  ;   '   | :  | :  |,'   :    ;   ")
message(STATUS "     / \\  \\  \\  ;   . |  ; |--' |   |  ./  ")
message(STATUS "    ;  /\\  \\  \\ |   : |  | ,    ;   : ;    ")
message(STATUS "  ./__;  \\  ;  \\|   : '  |/     |   ,/      ")
message(STATUS "  |   : / \\  \\  ;   | |`-'      '---'       ")
message(STATUS "  ;   |/   \\  ' |   ;/                       ")
message(STATUS "  `---'     `--`'---'                         ")
message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR     : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR     : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")
message(STATUS "** PUGIXML_INCLUDE_DIRS: ${PUGIXML_INCLUDE_DIRS}")


#
# add library: cyng_xml
#
include (lib.cmake)
add_library(cyng_xml ${GLOBAL_LIBRARY_TYPE} ${xml_lib})
add_library(cyng::xml ALIAS "cyng_xml")

target_include_directories(cyng_xml
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PUGIXML_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_xml PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug cyng_xml library
#
target_compile_definitions(cyng_xml 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_XML>
)

target_link_libraries(cyng_xml 
    PRIVATE 
        pugixml 
#        cyng::cyng_io
        Boost::boost 
)

