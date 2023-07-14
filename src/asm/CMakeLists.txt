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
# add library: cyng_asm
#
include (lib.cmake)
add_library(cyng_asm ${GLOBAL_LIBRARY_TYPE} ${asm_lib})
add_library(cyng::asm ALIAS "cyng_asm")

target_include_directories(cyng_asm
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/../../include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/../../include>
        $<BUILD_INTERFACE:${date_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE
        "$<$<PLATFORM_ID:Linux>:/usr/include>"
)

target_compile_features(cyng_asm PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug i/o library
#
target_compile_definitions(cyng_asm 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_IO>
        $<$<BOOL:${CYNG_USE_FEATURE_TESTING}>:__CYNG_USE_FEATURE_TESTING>
        $<$<CXX_COMPILER_ID:MSVC>:_WIN32_WINNT=0x0A00>
        $<$<CXX_COMPILER_ID:MSVC>:_SCL_SECURE_NO_WARNINGS>
        $<$<CXX_COMPILER_ID:MSVC>:WIN32_LEAN_AND_MEAN>   
)

target_link_libraries(cyng_asm 
    PRIVATE 
        Boost::boost 
)
