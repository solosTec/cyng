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
* and more
    * logging
    * crypto
    
## Introduction ##

* Current version is 0.1. Interfaces are almost stable now. 
* Linux (64 bit) are supported
* Windows 7 (64 bit) or higher are supported.
* Crosscompiling for Raspberry 3 is supported
* Requires [g++](https://gcc.gnu.org/) >= 4.8 or cl 19.00.24215.1 (this is VS 14.0) and [boost](http://www.boost.org/) >= 1.61.0

## How do I get set up? ##

* Buildsystem is based on [cmake](http://www.cmake.org/) >= 3.5
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





## License ##

CYY C++ library is free software under the terms of the [MIT License](https://github.com/solosTec/cyng/blob/master/LICENSE).

## Contact ##

Your pull requests are welcomed or you can post an [issue](https://github.com/solosTec/cyng/pulls)
