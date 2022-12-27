cmake_minimum_required (VERSION 3.14)
project(OBJECT)


message(STATUS "")                                                                                                              
#message(STATUS "  𝖔𝖇𝖏")                                 
message(STATUS "              ,---,              ")
message(STATUS "     ,---.  ,---.'|         .--. ")
message(STATUS "    '   ,'\\ |   | :       .--,`| ")
message(STATUS "   /   /   |:   : :       |  |.  ")
message(STATUS "  .   ; ,. ::     |,-.    '--`_  ")
message(STATUS "  '   | |: :|   : '  |    ,--,'| ")
message(STATUS "  '   | .; :|   |  / :    |  | ' ")
message(STATUS "  |   :    |'   : |: |    :  | | ")
message(STATUS "   \\   \\  / |   | '/ :  __|  : ' ")
message(STATUS "    `----'  |   :    |.'__/\\_: | ")
message(STATUS "            /    \\  / |   :    : ")
message(STATUS "            `-'----'   \\   \\  /  ")
message(STATUS "                        `--`-'   ")
message(STATUS "")                                                                                                                                                                        

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR  : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR  : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")


#
# add library: cyng_obj
#
include (lib.cmake)
add_library(cyng_obj ${GLOBAL_LIBRARY_TYPE} ${object_lib})
add_library(cyng::obj ALIAS "cyng_obj")

target_include_directories(cyng_obj
    PRIVATE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/../../include>
        $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_obj 
    PRIVATE 
        cxx_alignas
        cxx_std_20
        cxx_auto_type
        cxx_constexpr
        cxx_lambdas
        cxx_noexcept
        cxx_override
        cxx_range_for
        cxx_rvalue_references
        cxx_static_assert
        cxx_strong_enums
)

#
#   debug class object
#
target_compile_definitions(cyng_obj 
    PRIVATE 
        $<$<CONFIG:Debug>:__DEBUG_OBJECT>
        $<$<CXX_COMPILER_ID:MSVC>:_WIN32_WINNT=0x0A00>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>   
)

#target_link_libraries(cyng_obj 
#    PRIVATE 
#        "$<BUILD_INTERFACE:$<$<BOOL:${CYNG_USE_DATE_LIBRARY}>:date::date>>"
#        $<BUILD_INTERFACE:date::date>
#)
