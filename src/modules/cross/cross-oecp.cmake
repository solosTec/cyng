#
# Setup cross compilation environment with CMake.
#
# Usage: 
# cmake -DCMAKE_TOOLCHAIN_FILE=~/projects/PPC/cross.cmake ..
# cmake -DCYNG_BUILD_TEST:bool=OFF -DCYNG_CROSS_COMPILE:bool=ON -DCYNG_SSL_SUPPORT:bool=OFF -DCMAKE_TOOLCHAIN_FILE=~/projects/PPC/cross.cmake ..
# cmake -DNODE_BUILD_TEST:bool=OFF -DNODE_CROSS_COMPILE:bool=ON -DNODE_SSL_SUPPORT:bool=OFF -DCMAKE_TOOLCHAIN_FILE=~/projects/PPC/cross.cmake ..
#

#
# Define host system
#
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

#
# root path
#
SET(CROSS_ROOT $HOME/projects/PPC/oecp-bundle-1.1.0-1-g5acf3cce_solostec/sysroot-target)
set(CMAKE_ROOT_PATH ${CROSS_ROOT})
set(CMAKE_FIND_ROOT_PATH ${CROSS_ROOT})

#
# cross compiler location
#
set(CROSS_TOOLS /opt/OSELAS.Toolchain-2016.06.1/arm-v5te-linux-gnueabi/gcc-5.4.0-glibc-2.23-binutils-2.26-kernel-3.16.57-sanitized)
set(CMAKE_C_COMPILER ${CROSS_TOOLS}/bin/arm-v5te-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER ${CROSS_TOOLS}/bin/arm-v5te-linux-gnueabi-c++)

#
# include directories
#
include_directories(SYSTEM
	${CROSS_ROOT}/usr/include
	${CROSS_TOOLS}/arm-v5te-linux-gnueabi/include/c++/5.4.0
)

#
# link directories
#
link_directories(
	${CROSS_ROOT}/usr/lib
	${CROSS_TOOLS}/arm-v5te-linux-gnueabi/lib
)

#
# Boost support
#
set(BOOST_ROOT  $ENV{HOME}/projects/PPC/install)
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include" CACHE PATH "BOOST_INCLUDEDIR")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib" CACHE PATH "BOOST_LIBRARYDIR")

#
# OpenSSL support
#
set(OPENSSL_ROOT_DIR  $ENV{HOME}/projects/PPC/install/openssl)    
set(OPENSSL_INCLUDE_DIR ${OPENSSL_ROOT_DIR}/include)
set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.so)
set(OPENSLL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/lbssl.so)

#
# Use our definitions for compiler tools
#
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE NEVER)

#
# global compiler options
#
add_definitions("-DBOOST_ASIO_DISABLE_STD_FUTURE")

#
# some explanations:
# (1) -DBOOST_ASIO_DISABLE_STD_FUTURE is required to get rid of a bunch of compiler errors like: 'current_exception' is not a member of 'std'
# 	see https://github.com/chriskohlhoff/asio/issues/232
#

