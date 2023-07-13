cmake_minimum_required (VERSION 3.14)
project(ASM)


message(STATUS "")                                                           
message(STATUS "                                  ____    ")
message(STATUS "                                ,'  , `.  ")
message(STATUS "                             ,-+-,.' _ |  ")
message(STATUS "               .--.--.    ,-+-. ;   , ||  ")
message(STATUS "   ,--.--.    /  /    '  ,--.'|'   |  ||  ")
message(STATUS "  /       \\  |  :  /`./ |   |  ,', |  |, ")
message(STATUS " .--.  .-. | |  :  ;_   |   | /  | |--'   ")
message(STATUS "  \\__\\/: . .  \\  \\    `.|   : |  | ,  ")
message(STATUS "  ,\" .--.; |   `----.   \\   : |  |/     ")
message(STATUS " /  /  ,.  |  /  /`--'  /   | |`-'        ")
message(STATUS ";  :   .'   \\'--'.     /|   ;/           ")
message(STATUS "|  ,     .-./  `--'---' '---'             ")
message(STATUS " `--`---'                                 ")
                                         

message(STATUS "")
                                                           

message(STATUS "** CMAKE_SOURCE_DIR   : ${CMAKE_SOURCE_DIR}")
message(STATUS "** CMAKE_BINARY_DIR   : ${CMAKE_BINARY_DIR}")
message(STATUS "** PROJECT_SOURCE_DIR : ${PROJECT_SOURCE_DIR}")
message(STATUS "** PROJECT_BINARY_DIR : ${PROJECT_BINARY_DIR}")
message(STATUS "** CURRENT_SOURCE_DIR : ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_SOURCE_DIR     : ${${PROJECT_NAME}_SOURCE_DIR}")
message(STATUS "** ${PROJECT_NAME}_BINARY_DIR     : ${${PROJECT_NAME}_BINARY_DIR}")
message(STATUS "** Boost_LIBRARIES    : ${Boost_LIBRARIES}")
message(STATUS "** Boost_PROGRAM_OPTIONS_LIBRARY : ${Boost_PROGRAM_OPTIONS_LIBRARY}")

#
# add executable: asm
#
include (app.cmake)
add_executable(asm ${asm_app})
target_compile_features(asm PRIVATE cxx_std_20)

target_include_directories(asm
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

#
#   debug asm
#
target_compile_definitions(asm 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_ASM>
        $<$<CXX_COMPILER_ID:MSVC>:_WIN32_WINNT=0x0A00>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>   
)
#
# -Wno-psabi for gcc > 7.1
#
target_compile_options(asm 
    PRIVATE 
        $<$<AND:$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},7.1>,$<CXX_COMPILER_ID:GNU>>:-Wno-psabi>
        $<$<CXX_COMPILER_ID:MSVC>:/bigobj>
        $<$<CXX_COMPILER_ID:MSVC>:/MP>
)

#
#   dependencies: Boost
#
target_link_libraries(asm 
    PRIVATE 
		cyng::obj
		cyng::io
		cyng::log
		cyng::task
		cyng::parse
#        Boost::filesystem
        Boost::program_options
        Boost::filesystem
        "$<$<PLATFORM_ID:Linux>:pthread>"
)
