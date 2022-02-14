![GitHub](https://img.shields.io/github/license/solosTec/cyng.svg)

# CYNG C++ Library #

The CYNG library (pronunciation: /tʃɪŋ/) is mostly about support of dynamically typed values in C++, in several respects:

* core
    * Ability to wrap every C++ data type in a uniform object by taking advantage of the C++ template mechanism. No macros required.
    * Overcomes some restrictions of the [boost::any](http://www.boost.org/doc/libs/master/doc/html/any.html) data type
    * It's **not** intrusive. Set of small libraries you can use as needed.
    * Handy set of predefined data types.
    * Extensible for custom data types
    * Implementation of  the concept of an [empty value](https://en.wikipedia.org/wiki/Nullable_type)
    * Large set of unit tests
* io
    * Built in support for efficient binary serialization 
    * Formatted output
    * Extremely usefull to implement protocol parser and serializer.
* parser
    * Parsing all intrinsic data types
    * Extensible
* [JSON](http://json.org/)
    * Handling JSON objects like CYNG/C++ objects
    * Smooth integration of parsing and serialization 
* store
    * Simple key-value store
    * Threadsafe
    * Provides a lock hierarchy
* vm
    * A tiny VM extends dynamic data types by dynamic code execution
    * Comes with an assembler
    * Extensible by a library manager
    * Complete asynchronously
* async
    * Managing of single and periodic tasks
    * Load distribution to all available CPUs
    * Ease the burden of implementing thread safe tasks (The scheduler takes care for you)
    * Support of the [active object pattern](http://accu.org/index.php/journals/1956)
    * [Signal handling](http://solosTec.de/anchor/index.php/posts/a-unique-singleton)
    * new task library
* SQL
    * Generate SQL queries at runtime
	* C++ compiler checks SQL syntax
* db
	* Generalized interface to access databases
	* Support for ODBC and SQLite3
* and more
    * logging
    * crypto
    
## Introduction ##

* Current version is 0.9. Interfaces are stable now. 
* Linux (32/64 bit) are supported
* Windows 7/10 (64 bit) or higher are supported.
* Crosscompiling for Raspberry 3/4 is supported
* Requires [g++](https://gcc.gnu.org/) >= 8.x or cl 19.00.24215.1 (this is VS 14.0) and [boost](http://www.boost.org/) >= 1.75.0

## How do I get set up? ##

* Buildsystem is based on [cmake](http://www.cmake.org/) >= 3.12
* Download or clone from [github](https://github.com/solosTec/cyng.git)

To build CYNG, run:


```
#!shell

git clone https://github.com/solosTec/cyng.git
mkdir build
cd build
cmake ..
make -j4 all
sudo make install

```

To run a test, type

```
#!shell

./unitTest --report_level=detailed

```

To build with Visual Studion on Windows:

```
#!shell
mkdir projects
cd projects
cmake -Wno-dev -G "Visual Studio 15 2017 Win64" ..

```

This will generate a Solution file for Visual Studio 15. Open this file and select **Build All**. 
CMake may require a little help to find the Boost Library. In this case modify the variable *BOOST_ROOT* 
in CMakeList.txt to the correct path.


## Hints for cross compiling ##

To cross compile on Linux for [Raspberry Pi 3](https://www.raspberrypi.org/) use the [rpi-newer-crosstools](https://github.com/rvagg/rpi-newer-crosstools) with support for a decent gcc version (6.3.1). The original [toolchain](https://github.com/raspberrypi/tools) seems a little bit outdated.

### Boost ###

(1) download and extract latest [Boost library](https://dl.bintray.com/boostorg/release/1.69.0/source/boost_1_69_0.tar.bz2)

```
wget https://sourceforge.net/projects/boost/files/boost/1.77.0/boost_1_77_0.tar.bz2/download
tar xjvf boost_1_77_0.tar.bz2
cd boost_1_77_0
```

(2) bootstrap

```
./bootstrap.sh --with-libraries=filesystem,program_options,system,thread,timer,random,test,regex
```

Specify a none-standard installation path with --prefix=...


(3) edit project-config.jam

replace

```
    using gcc ; 
```

with one of the following lines (depends from the target platform)
```
    using gcc : arm : arm-rpi-linux-gnueabihf-g++
    using gcc : arm : arm-v5te-linux-gnueabi-c++ ;
```    

Select the required libraries (to save time):

```
libraries =  --with-filesystem --with-program_options --with-system --with-thread --with-timer --with-random --with-test;
```

Select a prefix:

```
option.set prefix : ${HOME}/projects/install ;
option.set exec-prefix : ${HOME}/projects/install ;
option.set libdir : ${HOME}/projects/install/lib ;
option.set includedir : ${HOME}/projects/install/include ;
```


(4) set path (examples - take the appropriate):

```
export PATH=$PATH:${HOME}/projects/rpi-newer-crosstools/x64-gcc-6.3.1/arm-rpi-linux-gnueabihf/bin/
export PATH=$PATH:/opt/OSELAS.Toolchain-2016.06.1/arm-v5te-linux-gnueabi/gcc-5.4.0-glibc-2.23-binutils-2.26-kernel-3.16.57-sanitized/bin/
```


(5) start generating

```
./b2 install toolset=gcc-arm --prefix=${HOME}/projects/install
```

### OpenSLL ###

(1) download and extract latest [OpenSSL library](http://www.openssl.org/source/openssl-1.1.1.tar.gz)

```
wget http://www.openssl.org/source/openssl-1.1.1.tar.gz
tar -xvzf openssl-1.1.1.tar.gz
cd openssl-1.1.1
```


(2) config (examples)

```
./Configure linux-generic32 shared --prefix=${HOME}/projects/install/openssl --openssldir=${HOME}/projects/install/openssl  --cross-compile-prefix=arm-rpi-linux-gnueabihf- PROCESSOR=ARM

./Configure linux-generic32 shared --prefix=${HOME}/projects/install/openssl --openssldir=${HOME}/projects/install/openssl  --cross-compile-prefix=arm-v5te-linux-gnueabi- PROCESSOR=ARM
```


(3) set path (examples - take the appropriate):

```
export PATH=$PATH:${HOME}/projects/rpi-newer-crosstools/x64-gcc-6.3.1/arm-rpi-linux-gnueabihf/bin/
export PATH=$PATH:/opt/OSELAS.Toolchain-2016.06.1/arm-v5te-linux-gnueabi/gcc-5.4.0-glibc-2.23-binutils-2.26-kernel-3.16.57-sanitized/bin/
```


(4) start generating

```
make
make install
```

### Set up cross compilation with CMake ###

Example for a CMake file cross.cmake 


```
#
# Define host system
#
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

#
# root path
#
SET(CROSS_ROOT $HOME/projects/sysroot-target)
set(CMAKE_ROOT_PATH ${CROSS_ROOT})
set(CMAKE_FIND_ROOT_PATH ${CROSS_ROOT})

#
# cross compiler location
# Example for OSELAS toolchain
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
set(BOOST_ROOT  $ENV{HOME}/projects/install)
set(BOOST_INCLUDEDIR "${BOOST_ROOT}/include" CACHE PATH "BOOST_INCLUDEDIR")
set(BOOST_LIBRARYDIR "${BOOST_ROOT}/lib" CACHE PATH "BOOST_LIBRARYDIR")

#
# OpenSSL support
#
set(OPENSSL_ROOT_DIR  $ENV{HOME}/projects/install/openssl)    
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
# this avoids a bunch of compiler errors
#
add_definitions("-DBOOST_ASIO_DISABLE_STD_FUTURE")

```

Generate make file

```

git clone https://github.com/solosTec/cyng.git
mkdir cross
cd cross
cmake -DCYNG_BUILD_TEST:bool=OFF -DCYNG_CROSS_COMPILE:bool=ON -DCYNG_SSL_SUPPORT:bool=ON -DCMAKE_TOOLCHAIN_FILE=~/projects/cross.cmake ..
make -j4 all
sudo make install

```

## License ##

CYNG C++ library is free software under the terms of the [MIT License](https://github.com/solosTec/cyng/blob/master/LICENSE).

## Contact ##

Your pull requests are welcomed or you can post an [issue](https://github.com/solosTec/cyng/pulls)

## Contributors ##

![Top Contributors](https://images.repography.com/23255267/solosTec/cyng/top-contributors/fb81572cf36d44c1278d8e54ea2926bd.svg)

