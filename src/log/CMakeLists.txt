cmake_minimum_required (VERSION 3.14)
project(LOG)


message(STATUS "")                                                           
message(STATUS "    ,--,                          ")
message(STATUS "  ,--.'|                          ")
message(STATUS "  |  | :     ,---.                ")
message(STATUS "  :  : '    '   ,'\\   ,----._,.  ")
message(STATUS "  |  ' |   /   /   | /   /  ' /   ")
message(STATUS "  '  | |  .   ; ,. :|   :     |   ")
message(STATUS "  |  | :  '   | |: :|   | .\\  .  ")
message(STATUS "  '  : |__'   | .; :.   ; ';  |   ")
message(STATUS "  |  | '.'|   :    |'   .   . |   ")
message(STATUS "  ;  :    ;\\   \\  /  `---`-'| | ")
message(STATUS "  |  ,   /  `----'   .'__/\\_: |  ")
message(STATUS "   ---`-'            |   :    :   ")
message(STATUS "                      \\   \\  /  ")
message(STATUS "                       `--`-'     ")
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
# add library: cyng_log
#
include (lib.cmake)
add_library(cyng_log ${GLOBAL_LIBRARY_TYPE} ${log_lib})
add_library(cyng::log ALIAS "cyng_log")

target_include_directories(cyng_log
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/../../include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_log PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

if (CMAKE_SYSTEM_NAME STREQUAL Linux)
	option(WITH_SYSTEMD "Include systemd support?" OFF)
	if (WITH_SYSTEMD)
		add_definitions("-DWITH_SYSTEMD")
		find_library(SYSTEMD_LIBRARY systemd)
	endif (WITH_SYSTEMD)
    message(STATUS "** journald support             : ${WITH_SYSTEMD}")     
endif (CMAKE_SYSTEM_NAME STREQUAL Linux)

#
#   debug logger
#
target_compile_definitions(cyng_log 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_LOG>
        "$<$<PLATFORM_ID:Linux>:APPENDER_SYS_LOG>"
        "$<$<BOOL:${WITH_SYSTEMD}>:APPENDER_JOURNALD>"
		"$<$<PLATFORM_ID:Windows>:APPENDER_EVENT_LOG>"
        $<$<CXX_COMPILER_ID:MSVC>:_WIN32_WINNT=0x0A00>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>   
)

target_link_libraries(cyng_log 
    PRIVATE 
        Boost::boost 
        "$<$<BOOL:${WITH_SYSTEMD}>:systemd>"
)
