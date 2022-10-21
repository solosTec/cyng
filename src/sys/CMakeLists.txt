cmake_minimum_required (VERSION 3.14)
project(SYS)


message(STATUS "")                                                           
                                  
message(STATUS "    .--.--.              .--.--.    ")
message(STATUS "   /  /    '       .--, /  /    '   ")
message(STATUS "  |  :  /`./     /_ ./||  :  /`./   ")
message(STATUS "  |  :  ;_    , ' , ' :|  :  ;_     ")
message(STATUS "   \\  \\    `./___/ \\: | \\  \\    `.  ")
message(STATUS "    `----.   \\.  \\  ' |  `----.   \\ ")
message(STATUS "   /  /`--'  / \\  ;   : /  /`--'  / ")
message(STATUS "  '--'.     /   \\  \\  ;'--'.     /  ")
message(STATUS "    `--'---'     :  \\  \\ `--'---'   ")
message(STATUS "                  \\  ' ;            ")
message(STATUS "                   `--`             ")

message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR     : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR     : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")

#
# add library: cyng_sys
#
include (lib.cmake)
add_library(cyng_sys ${GLOBAL_LIBRARY_TYPE} ${sys_lib})
add_library(cyng::sys ALIAS "cyng_sys")

target_include_directories(cyng_sys
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
        $<BUILD_INTERFACE:${date_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_sys PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug sys library
#   __CYNG_ARCHITECTURE contains the target architecture
#
target_compile_definitions(cyng_sys 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_SYS>
        $<IF:$<BOOL:${CMAKE_CROSSCOMPILING}>,__CROSS_PLATFORM,__NATIVE_PLATFORM>
        $<$<BOOL:${CYNG_USE_FEATURE_TESTING}>:__CYNG_USE_FEATURE_TESTING>
        $<$<BOOL:${CYNG_USE_DATE_LIBRARY}>:__CYNG_USE_DATE_LIBRARY> 
        "__CYNG_ARCHITECTURE=${CMAKE_SYSTEM_PROCESSOR}"
)

# -Wno-psabi if gcc 8.x
target_compile_options(cyng_sys 
    PRIVATE 
        $<$<AND:$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},7.1>,$<CXX_COMPILER_ID:GNU>>:-Wno-psabi>
)

#
#   There is a dependency to parse library
#   dependencies: cyng_parse
#   CYNG_USE_DATE_LIBRARY
target_link_libraries(cyng_sys 
    PRIVATE 
        "$<$<PLATFORM_ID:Linux>:cyng_parse>"
        "$<$<PLATFORM_ID:Linux>:${Boost_LOCALE_LIBRARY}>"
#        "$<BUILD_INTERFACE:date::date>"
#        "$<$<AND:0,${CYNG_USE_DATE_LIBRARY}>:date::date>"
#        "$<$<BOOL:${CYNG_USE_DATE_LIBRARY}>:date::date>"
        "$<BUILD_INTERFACE:$<$<BOOL:${CYNG_USE_DATE_LIBRARY}>:date::date>>"
)
  