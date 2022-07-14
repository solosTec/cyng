cmake_minimum_required (VERSION 3.14)
project(RND)


message(STATUS "")                                                           
message(STATUS "       ***** ***       ***** *     **          ***** **    ")
message(STATUS "    ******  * **    ******  **    **** *    ******  ***    ")
message(STATUS "   **   *  *  **   **   *  * **    ****   **    *  * ***   ")
message(STATUS "  *    *  *   **  *    *  *  **    * *   *     *  *   ***  ")
message(STATUS "      *  *    *       *  *    **   *          *  *     *** ")
message(STATUS "     ** **   *       ** **    **   *         ** **      ** ")
message(STATUS "     ** **  *        ** **     **  *         ** **      ** ")
message(STATUS "     ** ****         ** **     **  *         ** **      ** ")
message(STATUS "     ** **  ***      ** **      ** *         ** **      ** ")
message(STATUS "     ** **    **     ** **      ** *         ** **      ** ")
message(STATUS "     *  **    **     *  **       ***         *  **      ** ")
message(STATUS "        *     **        *        ***            *       *  ")
message(STATUS "    ****      ***   ****          **       *****       *   ")
message(STATUS "   *  ****    **   *  *****               *   *********    ")
message(STATUS "  *    **     *   *     **               *       ****      ")
message(STATUS "  *               *                      *                 ")
message(STATUS "   **              **                     **               ")
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
# add library: cyng_rnd
#
include (lib.cmake)
add_library(cyng_rnd ${GLOBAL_LIBRARY_TYPE} ${rnd_lib})
add_library(cyng::rnd ALIAS "cyng_rnd")

target_include_directories(cyng_rnd
    PUBLIC 
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_features(cyng_rnd PRIVATE cxx_std_20 cxx_constexpr cxx_lambdas)

#
#   debug rnd library
#
target_compile_definitions(cyng_rnd 
    PRIVATE 
        $<$<CONFIG:Debug>:_DEBUG_RND>
)

target_link_libraries(cyng_rnd 
    PRIVATE 
        Boost::boost 
)
