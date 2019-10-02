#
# BOOST_VER is "1_67", "1_68", "1_69", "1_70" or "1_71"
#
function(windows_boost_fix BOOST_VER)
#
#	select MSVC version of Boost library
#
	if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "19.21")
		set(_vc "14.1")
		set(_vcc "141")
	else()
		set(_vc "14.2")
		set(_vcc "142")
	endif()
	message(STATUS "** select  MSVC    : ${_vc}")

	set(BOOST_ROOT "C:/local/boost_${BOOST_VER}_0" CACHE PATH "BOOST_ROOT")
	set(BOOST_LIBRARYDIR "C:/local/boost_${BOOST_VER}_0/lib64-msvc-${_vc}" CACHE PATH "BOOST_LIBRARYDIR")
	set(BOOST_INCLUDE_DIR "C:/local/boost_${BOOST_VER}_0" CACHE PATH "BOOST_INCLUDE_DIR")
	set(BOOST_DIR "C:/local/boost_${BOOST_VER}" CACHE PATH "BOOST_DIR")
	message(STATUS "** Search Boost    : overwrite with ${BOOST_ROOT}")
	message(STATUS "** Boost Library   : ${BOOST_LIBRARYDIR}")

	set(Boost_ATOMIC_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_atomic-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_DATE_TIME_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_date_time-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_FILESYSTEM_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_filesystem-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_PROGRAM_OPTIONS_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_program_options-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_PROGRAM_OPTIONS_LIBRARY_RELEASE "${BOOST_LIBRARYDIR}/boost_program_options-vc${_vcc}-mt-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_RANDOM_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_random-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_REGEX_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_regex-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_SYSTEM_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_system-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_SYSTEM_LIBRARY_RELEASE "${BOOST_LIBRARYDIR}/libboost_system-vc${_vcc}-mt-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_THREAD_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_thread-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)
	set(Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG "${BOOST_LIBRARYDIR}/libboost_unit_test_framework-vc${_vcc}-mt-gd-x64-${BOOST_VER}.lib" PARENT_SCOPE)

endfunction()

#
#	setup Boost library 1.71.0, 1.70.0, 1.69.0, 1.68.0 or 1.67.0
#
if(NOT ${PROJECT_NAME}_CROSS_COMPILE)
    if(UNIX)
		#
		# search for installation in path ~/projects/boost_X_YY_Z/install
		#
        if(EXISTS "$ENV{HOME}/projects/boost_1_71_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_71_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_71_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_70_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_70_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_70_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_69_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_69_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_69_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_68_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_68_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_68_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_67_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_67_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_67_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        elseif(EXISTS "$ENV{HOME}/projects/boost_1_66_0/install")
            set(BOOST_ROOT "$ENV{HOME}/projects/boost_1_66_0/install" CACHE PATH "BOOST_ROOT")
            set(BOOST_LIBRARYDIR "$ENV{HOME}/projects/boost_1_66_0/install/lib" CACHE PATH "BOOST_LIBRARYDIR")
            message(STATUS "** Search Boost    : overwrite BOOST_ROOT with ${BOOST_ROOT}")
        endif()
    elseif(WIN32)
        if(EXISTS "C:/local/boost_1_71_0")
            windows_boost_fix("1_71")
        elseif(EXISTS "C:/local/boost_1_70_0")
            windows_boost_fix("1_70")
        elseif(EXISTS "C:/local/boost_1_69_0")
            windows_boost_fix("1_69")
        elseif(EXISTS "C:/local/boost_1_68_0")
            windows_boost_fix("1_68")
        elseif(EXISTS "C:/local/boost_1_67_0")
            windows_boost_fix("1_67")
        endif()
    endif(UNIX)
endif()

set(Boost_ADDITIONAL_VERSIONS "1.67.0" "1.68.0" "1.69.0" "1.70.0" "1.71.0")
message(STATUS "** Search Boost    : ${Boost_ADDITIONAL_VERSIONS}")
find_package(Boost 1.67 REQUIRED COMPONENTS thread system filesystem program_options random unit_test_framework regex)

if(Boost_FOUND)

	message(STATUS "** Boost Version    : ${Boost_VERSION}")
	message(STATUS "** Boost Include    : ${Boost_INCLUDE_DIRS}")
	message(STATUS "** Boost Path       : ${Boost_LIBRARY_DIRS}")
	message(STATUS "** Boost Libraries  : ${Boost_LIBRARIES}")

	if(Boost_VERSION VERSION_LESS "1.66.0")
		#
		# When working with a Boost version prior 1.66.0 newer versions of Asio and Beast 
		# are required.
		#
		message(WARING "** Newer versions of Asio and Beast are required")

		if(UNIX)
			include_directories($ENV{HOME}/projects/beast/include)
			include_directories($ENV{HOME}/projects/asio/include)
		endif()
	endif()

	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
	
	set(${PROJECT_NAME}_BOOST_LIBRARY ${PROJECT_NAME}_BOOST_LIBRARY_FOUND)
	set(${PROJECT_NAME}_BOOST_VERSION ${PROJECT_NAME}_BOOST_${Boost_VERSION})
	
	# support for unicode
	# increase MPL list size for Boost.Variant
	# possible values are  30, 40 or 50 
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_SPIRIT_UNICODE -DBOOST_MPL_CFG_NO_PREPROCESSED_HEADERS -DBOOST_MPL_LIMIT_LIST_SIZE=50 -DFUSION_MAX_VECTOR_SIZE=50 -DBOOST_ASIO_HAS_MOVE")
	
else()
	set(${PROJECT_NAME}_BOOST_LIBRARY ${PROJECT_NAME}_BOOST_LIBRARY_NOT_FOUND)
endif(Boost_FOUND)
