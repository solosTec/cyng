cmake_minimum_required (VERSION 3.14)
project(DB)


message(STATUS "")                                                           
message(STATUS "        ,---,  ,---,    ")
message(STATUS "      ,---.'|,---.'|    ")
message(STATUS "      |   | :|   | :    ")
message(STATUS "      |   | |:   : :    ")
message(STATUS "    ,--.__| |:     |,-. ")
message(STATUS "   /   ,'   ||   : '  | ")
message(STATUS "  .   '  /  ||   |  / : ")
message(STATUS "  '   ; |:  |'   : |: | ")
message(STATUS "  |   | '/  '|   | '/ : ")
message(STATUS "  |   :    :||   :    | ")
message(STATUS "   \\   \\  /  /    \\  /  ")
message(STATUS "    `----'   `-'----'   ")
message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR      : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR      : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** SQLite3_INCLUDE_DIR: ${CMAKE_SOURCE_DIR}/${SQLite3_INCLUDE_DIR}")
message(STATUS "** Library type       : ${GLOBAL_LIBRARY_TYPE}")


#
# add library: cyng_db
#
include (lib.cmake)
add_library(cyng_db ${GLOBAL_LIBRARY_TYPE} ${db_lib})
add_library(cyng::db ALIAS "cyng_db")

target_include_directories(cyng_db
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/../../include>
    PRIVATE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
)

target_compile_features(cyng_db PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug db library
#
target_compile_definitions(cyng_db 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_DB>
        $<$<CXX_COMPILER_ID:MSVC>:_WIN32_WINNT=0x0A00>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>   
)
#
# -Wno-psabi for gcc > 7.1
#
target_compile_options(cyng_db 
    PRIVATE 
        $<$<AND:$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},7.1>,$<CXX_COMPILER_ID:GNU>>:-Wno-psabi>
        $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
        $<$<CXX_COMPILER_ID:MSVC>:/MP>
)

#
# CYNG_ODBC_CONNECTOR
#   dependencies: cyng_sql
#
target_link_libraries(cyng_db 
    PRIVATE 
        "$<$<PLATFORM_ID:Linux>:cyng_sql>"
        Boost::boost 
		"$<$<PLATFORM_ID:Windows>:odbc32>"
        $<$<AND:$<BOOL:${CYNG_ODBC_CONNECTOR}>,$<PLATFORM_ID:Linux>>:odbc>
)
