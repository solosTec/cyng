# Dependencies
find_package(Boost 1.75 REQUIRED COMPONENTS unit_test_framework)

add_executable(unit_test 
    main.cpp
    src/test_core.cpp
    src/test_db.cpp
    src/test_io.cpp
    src/test_vm.cpp
    src/test_store.cpp
    src/test_log.cpp
    src/test_sys.cpp
    src/test_sql.cpp
    src/test_parse.cpp
    src/test_task.cpp
    src/demo_task.cpp
    src/demo_task_ref.cpp
    src/test_net.cpp)

target_compile_features(unit_test PRIVATE cxx_std_20)

target_compile_definitions(unit_test
    PRIVATE 
        BOOST_TEST_DYN_LINK 
        BOOST_ASIO_HAS_MOVE 
        $<$<CONFIG:Debug>:_DEBUG_TEST>
)

#
# -Wno-psabi for gcc > 7.1
# add_compile_options(/utf-8)
#
target_compile_options(unit_test 
    PRIVATE 
        $<$<AND:$<VERSION_GREATER_EQUAL:${CMAKE_CXX_COMPILER_VERSION},7.1>,$<CXX_COMPILER_ID:GNU>>:-Wno-psabi>
        $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

target_include_directories(unit_test
    PRIVATE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
message(STATUS "** Boost_UNIT_TEST_FRAMEWORK_LIBRARY      : ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG}")

target_link_directories(unit_test
    PRIVATE
        ${Boost_LIBRARY_DIRS}
)

#
#   Linking against the SQLite C library requires "-ldl"
#
target_link_libraries(unit_test 
    PRIVATE
        cyng::obj
        cyng::task
        cyng::io
        cyng::vm
        cyng::log
        cyng::sql
        cyng::db
        cyng::sqlite3
        cyng::store
        cyng::sys
        cyng::parse
        cyng::net
        "$<$<PLATFORM_ID:Linux>:pthread;${CMAKE_DL_LIBS}>"
        "$<$<PLATFORM_ID:Linux>:Boost::locale>"
        "$<$<PLATFORM_ID:Windows>:odbc32>"
        Boost::unit_test_framework
        $<$<AND:$<VERSION_LESS_EQUAL:${CMAKE_CXX_COMPILER_VERSION},9.1>,$<CXX_COMPILER_ID:GNU>>:stdc++fs>        
)

# include(CTest)
enable_testing()

add_test(DemoTest unit_test)

