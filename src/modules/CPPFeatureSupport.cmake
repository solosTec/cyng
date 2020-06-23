#
# Set a list of definitions depending available C++ features.
# Available for gcc, clang and msvc
# see https://en.cppreference.com/w/cpp/compiler_support
# see https://clang.llvm.org/cxx_status.html
# see https://devblogs.microsoft.com/cppblog/announcing-msvc-conforms-to-the-c-standard/
# see https://docs.microsoft.com/en-us/cpp/overview/visual-cpp-language-conformance?view=vs-2019
# see https://gcc.gnu.org/projects/cxx-status.html
#
# MSVC++ 14.0  (Visual Studio 2015 version 14.0)  _MSC_VER == 1900 
# MSVC++ 14.1  (Visual Studio 2017 version 15.0)  _MSC_VER == 1910 
# MSVC++ 14.11 (Visual Studio 2017 version 15.3)  _MSC_VER == 1911 
# MSVC++ 14.12 (Visual Studio 2017 version 15.5)  _MSC_VER == 1912 
# MSVC++ 14.13 (Visual Studio 2017 version 15.6)  _MSC_VER == 1913 
# MSVC++ 14.14 (Visual Studio 2017 version 15.7)  _MSC_VER == 1914 
# MSVC++ 14.15 (Visual Studio 2017 version 15.8)  _MSC_VER == 1915 
# MSVC++ 14.16 (Visual Studio 2017 version 15.9)  _MSC_VER == 1916 
# MSVC++ 14.2  (Visual Studio 2019 Version 16.0)  _MSC_VER == 1920 
# MSVC++ 14.21 (Visual Studio 2019 Version 16.1)  _MSC_VER == 1921 
# MSVC++ 14.22 (Visual Studio 2019 Version 16.2)  _MSC_VER == 1922 
# MSVC++ 14.23 (Visual Studio 2019 Version 16.3)  _MSC_VER == 1923 
# MSVC++ 14.24 (Visual Studio 2019 Version 16.4)  _MSC_VER == 1924 
#
# 19.21.27702.2 => Visual Studio 2019 version 16.1.6
# 19.24.28316 => Visual Studio 2019 version 16.4.5
#

function(emit_compiler_support PAPER DESCR MIN_VER)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL ${MIN_VER})
        set(${PROJECT_NAME}_CPP_SUPPORT_${PAPER} ON)
        add_definitions(-D__CPP_SUPPORT_${PAPER})
        message(STATUS "** ${CMAKE_CXX_COMPILER_ID} compiler ${CMAKE_CXX_COMPILER_VERSION} supports: ${PAPER} - ${DESCR}")
#        message(STATUS "** __CPP_SUPPORT_${PAPER}")
    else()
        message(STATUS "** ${CMAKE_CXX_COMPILER_ID} compiler ${CMAKE_CXX_COMPILER_VERSION} don't support: ${PAPER} - ${DESCR}")
    endif()
endfunction()

function(define_compiler_support PAPER DESCR MIN_VER_GCC MIN_VER_CLANG MIN_VER_MSVC MIN_VER_INTEL)

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
        emit_compiler_support(${PAPER} ${DESCR} ${MIN_VER_MSVC})
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
        emit_compiler_support(${PAPER} ${DESCR} ${MIN_VER_GCC})
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        emit_compiler_support(${PAPER} ${DESCR} ${MIN_VER_CLANG})
    elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
        emit_compiler_support(${PAPER} ${DESCR} ${MIN_VER_INTEL})
    endif()
    
endfunction()

#
#	setup C++ compiler
#

#
#   C++2a features
#
message(STATUS "** test ${CMAKE_CXX_COMPILER_ID} compiler ${CMAKE_CXX_COMPILER_VERSION} for C++2a features")
define_compiler_support("P1099R5" "using enums" "999" "999" "19.24" "999")
define_compiler_support("P0409R2" "Allow lambda-capture [=, this]" "8.0" "6.0" "19.21" "999.9")
define_compiler_support("P0306R4" "__VA_OPT__" "8.0" "6.0" "999" "999")
define_compiler_support("P0329R4" "Designated initializers" "8.0" "10.0" "19.21" "999")
define_compiler_support("P0428R2" "template-parameter-list for generic lambdas" "8.0" "9.0" "999" "999")
define_compiler_support("P0683R1" "Default member initializers for bit-fields" "8.0" "6.0" "999" "999")
define_compiler_support("P0702R1" "Initializer list constructors in class template argument deduction" "8.0" "6.0" "999" "999")
define_compiler_support("P0704R1" "const&-qualified pointers to members" "8.0" "6.0" "999" "999")
# 	see P0857R0, P1084R2, P1141R2, P0848R3, P1616R1, P1452R2, P1972R0, P1980R0
define_compiler_support("P0734R0" "Concepts" "10.0" "10.0" "19.23" "999")
define_compiler_support("P0315R4" "Lambdas in unevaluated contexts" "9.0" "999" "999" "999")
# see P1120R0, P1185R2, P1186R3, P1630R1, P1946R0, P1959R0
define_compiler_support("P0515R3" "Three-way comparison operator" "10.0" "10.0" "19.20" "999")
define_compiler_support("P0588R1" "Simplifying implicit lambda capture" "8.0" "999" "999" "999")
define_compiler_support("P0614R1" "init-statements for range-based for" "9.0" "8.0" "999" "999")
define_compiler_support("P0624R2" "Default constructible and assignable stateless lambdas" "9.0" "8.0" "999" "999")
define_compiler_support("P0641R2" "const mismatch with defaulted copy constructor" "9.0" "8.0" "19.21" "999")
define_compiler_support("P0692R1" "Access checking on specializations" "1.0" "8.0" "999" "999")
define_compiler_support("P0846R0" "ADL and function templates that are not visible" "9.0" "9.0" "999" "999")
define_compiler_support("P0859R0" "Less eager instantiation of constexpr functions" "9.0" "999" "999" "999")
define_compiler_support("P0479R5" "Attributes [[likely]] and [[unlikely]]" "9.0" "999.9" "999" "999")
define_compiler_support("P0634R3" "Make typename more optional" "9.0" "999" "999" "999")
define_compiler_support("P0780R2" "Pack expansion in lambda init-capture" "9.0" "9.0" "999" "999")
define_compiler_support("P0840R2" "Attribute [[no_unique_address]]" "9.0" "9.0" "999" "999")
define_compiler_support("P0542R5" "Contracts" "999" "999" "999" "999")
define_compiler_support("P0722R3" "Destroying operator delete" "9.0" "6.0" "999" "999")
# see P1907R1
define_compiler_support("P0732R2" "Class types in non-type template parameters" "9.0" "999" "999" "999")
define_compiler_support("P0892R2" "explicit(bool)" "9.0" "9.0" "19.24" "999")
define_compiler_support("P0941R2" "Integrating feature-test macros" "5.0" "3.4" "19.15" "999")
define_compiler_support("P1008R1" "Prohibit aggregates with user-declared constructors" "9.0" "8.0" "10.0" "999")
define_compiler_support("P1064R0" "constexpr virtual function" "9.0" "999" "999" "999")
define_compiler_support("P0482R6" "char8_t: A type for UTF-8 characters and strings" "9.0" "7.0" "19.22" "999")
define_compiler_support("P0595R2" "std::is_constant_evaluated()" "9.0" "9.0" "999" "999")
define_compiler_support("P1002R1" "constexpr try-catch blocks" "9.0" "8.0" "999" "999")
# see P1937R2
define_compiler_support("P1073R3" "Immediate function" "10.0" "999" "999" "999")
define_compiler_support("P1094R2" "Nested inline namespaces" "9.0" "8.0" "999" "999")
# see P1002R1 P1327R1 P1330R0 P1331R2 P1668R1 P0784R7
define_compiler_support("P1327R1" "constexpr dynamic_cast and polymorphic typeid" "10.0" "10.0" "999" "999")
define_compiler_support("P1330R0" "Changing the active member of a union inside constexpr" "9.0" "9.0" "14.0" "999")
# see P1381R1 (clang partial)
define_compiler_support("P1091R3" "Structured binding extensions (to be more like variable declarations)" "8.0" "10.0" "19.24" "999")
define_compiler_support("P1041R4" "Stronger Unicode requirements" "999" "1.0" "999" "999")
define_compiler_support("P0960R3" "Parenthesized initialization of aggregates" "999" "999" "999" "999")
define_compiler_support("P1103R3" "Modules" "999" "8.0" "999" "999")
define_compiler_support("P0912R5" "Coroutines" "999" "8.0" "999" "999")
define_compiler_support("P0463R1" "std::endian" "8.0" "7.0" "999" "999")
define_compiler_support("P0550R2" "std::remove_cvref" "9.0" "6.0" "19.20" "999")
define_compiler_support("P0674R1" "Extending std::make_shared to support arrays" "999" "999" "999" "999")
define_compiler_support("P0020R6" "Floating point atomic" "999" "999" "999" "999")
define_compiler_support("P0053R7" "Synchronized buffered ostream" "999" "999" "999" "999")
define_compiler_support("P0202R3" "constexpr for <algorithm> and <utility>" "999" "8.0" "999" "999")
define_compiler_support("P0415R1" "More constexpr for <complex>" "9.0" "999" "999" "999")
define_compiler_support("P0457R2" "String prefix and suffix checking" "9.0" "6.0" "999" "999")
define_compiler_support("P0515R3" "Library support for operator<=> <compare>" "10.0" "8.0" "19.20" "999")
define_compiler_support("P0653R2" "Utility to convert a pointer to a raw pointer" "8.0" "6.0" "999" "999")
# unsupported yet
#define_compiler_support("P0718R2" "Atomic shared_ptr and weak_ptr" "999" "999" "999" "999")
define_compiler_support("P0122R7" "std::span" "999" "7.0" "999" "999")
define_compiler_support("P0355R7" "Calendar and timezone" "999" "7.0" "999" "999")
define_compiler_support("P0754R2" "<version>" "9.0" "7.0" "999" "999")
define_compiler_support("P0019R8" "std::atomic_ref" "999" "999" "999" "999")
define_compiler_support("P0556R3" "Integral power-of-2 operations" "9.0" "999" "999" "999")
# unsupported yet
#define_compiler_support("P0476R2" "std::bit_cast()" "999" "999" "999" "999")
define_compiler_support("P0722R3" "std::destroying_delete" "9.0" "999" "999" "999")
define_compiler_support("P0898R3" "Concepts library" "999" "999" "999" "999")
define_compiler_support("P1209R0" "Consistent container erasure" "9.0" "8.0" "999" "999")
define_compiler_support("P1143R2" "constinit" "10.0" "10.0" "999" "999")

#
#   C++17 features
#	g++ -std=c++17
#	cl /std:c++latest
#
message(STATUS "** test ${CMAKE_CXX_COMPILER_ID} compiler ${CMAKE_CXX_COMPILER_VERSION} for C++17 features")
define_compiler_support("N3291" "reverse_copy" "8.1" "6.0" "19.21" "999")
define_compiler_support("N3911" "std::void_t" "6.0" "3.6" "19.0" "999")
# not really sure about support of N3915 - maybe C++ 2015 CTP, GCC 4.9.2, and Clang 3.5 are sufficient
define_compiler_support("N3915" "apply() call a function with arguments from a tuple" "7.1" "5.0" "19.14" "999")
define_compiler_support("N3922" "New auto rules for direct-list-initialization" "5.0" "3.8" "19.0" "17.0")
define_compiler_support("N3928" "static_assert with no message" "6.0" "3.5" "19.10" "18.0")
define_compiler_support("N4051" "typename in a template template parameter" "5.0" "3.5" "19.0" "17.0")
define_compiler_support("N4086" "Removing trigraphs" "5.0" "3.5" "16.0" "999")
define_compiler_support("N4230" "Nested namespace definition" "6.0" "3.6" "19.0" "17.0")
define_compiler_support("N4266" "Attributes for namespaces and enumerators" "6.0" "3.6" "19.0" "17.0")
define_compiler_support("N4267" "u8 character literals" "6.0" "3.6" "19.0" "17.0")
define_compiler_support("N4268" "Allow constant evaluation for all non-type template arguments" "6.0" "3.6" "19.12" "999")
# P0036R0 (clang since 3.9)
define_compiler_support("N4295" "Fold Expressions" "6.0" "3.6" "19.12" "19.0")
define_compiler_support("P0036R0" "Unary fold expressions and empty parameter packs" "6.0" "3.9" "19.12" "999")
# not a feature
#define_compiler_support("P0001R1" "Remove Deprecated Use of the register Keyword" "7.0" "3.8" "19.11" "18.0")
# not a feature
#define_compiler_support("P0002R1" "Remove Deprecated operator++(bool)" "7.0" "3.8" "19.11" "18.0")
define_compiler_support("P0012R1" "Make exception specifications part of the type system" "7.0" "4.0" "19.12" "19.0")
define_compiler_support("P0017R1" "Aggregate initialization of classes with base classes" "7.0" "3.9" "19.14" "999")
define_compiler_support("P0061R1" "__has_include in preprocessor conditionals" "5.0" "1.0" "19.11" "18.0")
define_compiler_support("P0136R1" "New specification for inheriting constructors (DR1941 et al)" "7.0" "3.9" "19.14" "999")
define_compiler_support("P0018R3" "Lambda capture of *this" "7.0" "3.9" "19.11" "19.0")
define_compiler_support("P0138R2" "Direct-list-initialization of enumerations" "7.0" "3.9" "19.11" "18.0")
define_compiler_support("P0170R1" "constexpr lambda expressions" "7.0" "5.0" "19.11" "18.1")
define_compiler_support("P0184R0" "Differing begin and end types in range-based for" "6.0" "3.9" "19.10" "18.0")
define_compiler_support("P0188R1" "[[fallthrough]] attribute" "7.0" "3.9" "19.10" "18.0")
define_compiler_support("P0189R1" "[[nodiscard]] attribute" "7.0" "3.9" "19.11" "18.0")
define_compiler_support("P0212R1" "[[maybe_unused]] attribute" "7.0" "3.9" "19.11" "18.0")
define_compiler_support("P0245R1" "Hexadecimal floating-point literals" "3.0" "1.0" "19.11" "18.0")
define_compiler_support("P0028R4" "Using attribute namespaces without repetition" "7.0" "3.9" "19.11" "18.0")
define_compiler_support("P0035R4" "Dynamic memory allocation for over-aligned data" "7.0" "4.0" "19.12" "999")
define_compiler_support("P0091R3" "Class template argument deduction" "7.0" "5.0" "19.14" "19.1")
define_compiler_support("P0127R2" "Non-type template parameters with auto type" "7.0" "4.0" "19.14" "19.1")
define_compiler_support("P0135R1" "Guaranteed copy elision" "7.0" "4.0" "19.13" "19.1")
define_compiler_support("P0137R1" "Replacement of class objects containing reference members" "7.0" "6.0" "19.14" "999")
define_compiler_support("P0145R3" "Stricter expression evaluation order" "7.0" "4.0" "19.14" "999")
# there is a bug in gcc 7.2.0 (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=81888) so this feature is effecive available since gcc 8.0
# see P0961R1 and P0969R0 too
define_compiler_support("P0217R3" "Structured Bindings (like auto [...] = f())" "8.0" "4.0" "19.11" "18.1")
define_compiler_support("P0283R2" "Ignore unknown attributes" "1.0" "3.9" "19.11" "18.0")
define_compiler_support("P0292R2" "constexpr if statements" "7.0" "3.9" "19.11" "19.0")
define_compiler_support("P0305R1" "init-statements for if and switch" "7.0" "3.9" "19.11" "18.0")
define_compiler_support("P0386R2" "Inline variables" "7.0" "3.9" "19.12" "19.0")
define_compiler_support("P0003R5" "Removing Deprecated Exception Specifications from C++17" "7.0" "4.0" "19.10" "19.0")
define_compiler_support("P0195R2" "Pack expansions in using-declarations" "7.0" "4.0" "19.14" "999")
define_compiler_support("P0522R0" "DR: Matching of template template-arguments excludes compatible templates" "7.0" "4.0" "19.12" "999")
# see P0220R1 too
define_compiler_support("N3921" "std::string_view" "7.0" "4.0" "19.10" "999")
define_compiler_support("N4259" "std::uncaught_exceptions()" "6.0" "3.7" "19.0" "999")
define_compiler_support("N4387" "Improving std::pair and std::tuple" "6.0" "4.0" "19.0" "999")
# see also N4315, N4065, N4031, N3824
define_compiler_support("N4391" "Library Fundamentals 2 TS - make_array" "7.1" "4.0" "19.20" "999")
# compare with N3659
define_compiler_support("N4508" "std::shared_mutex (untimed)" "6.1" "3.4" "19.0" "999")
define_compiler_support("P0024R2" "Standardization of Parallelism TS" "999" "999" "19.14" "18.0")
define_compiler_support("P0154R1" "Hardware interference size" "999" "999" "19.11" "999")
# gcc 8 should support this but I've git massive problems when cross compiling
define_compiler_support("P0218R1" "File system library" "8.0" "7.0" "19.14" "999")
define_compiler_support("P0220R1" "std::any / std::optional" "7.1" "4.0" "19.10" "999")
define_compiler_support("P0226R1" "Mathematical special functions" "7.0" "999" "19.14" "999")
define_compiler_support("P0063R3" "C++17 should refer to C11 instead of C99" "9.0" "7.0" "19.0" "999")
define_compiler_support("P0083R3" "Splicing Maps and Sets" "7.0" "8.0" "19.12" "999")
define_compiler_support("P0088R3" "std::variant" "7.1" "4.0" "19.10" "999")
define_compiler_support("P0067R5" "Elementary string conversions" "8.0" "999" "19.11" "999")
define_compiler_support("P0298R3" "std::byte" "7.0" "5.0" "19.11" "999")
define_compiler_support("P0156R0" "std::scoped_lock" "7.0" "5.0" "19.11" "999")

#
#   C++14 features
#	g++ -std=c++14
#	cl /std:c++latest
#
message(STATUS "** test ${CMAKE_CXX_COMPILER_ID} compiler ${CMAKE_CXX_COMPILER_VERSION} for C++14 features")
define_compiler_support("N3323" "Tweaked wording for contextual conversions" "4.9" "3.4" "18.0" "16.0")
define_compiler_support("N3472" "Binary literals" "4.9" "2.9" "19.0" "14.0")
define_compiler_support("N3638" "decltype(auto), Return type deduction for normal functions" "4.9" "3.4" "19.0" "15.0")
define_compiler_support("N3648" "Initialized/Generalized lambda captures (init-capture)" "4.9" "3.4" "19.0" "15.0")
define_compiler_support("N3649" "Generic (polymorphic) lambda expressions" "4.9" "3.4" "19.0" "16.0")
define_compiler_support("N3651" "Variable templates" "5.0" "3.4" "19.0" "17.0")
define_compiler_support("N3652" "Extended constexpr" "5.0" "3.4" "19.10" "17.0")
define_compiler_support("N3653" "Member initializers and aggregates (NSDMI)" "5.0" "3.3" "19.10" "16.0")
define_compiler_support("N3664" "Clarifying memory allocation (avoiding/fusing allocations)" "999" "3.4" "999" "999")
define_compiler_support("N3760" "[[deprecated]] attribute" "4.9" "3.4" "19.0" "16.0")
define_compiler_support("N3778" "Sized deallocation" "5.0" "3.4" "19.0" "17.0")
define_compiler_support("N3781" "Single quote as digit separator" "4.9" "3.4" "19.0" "16.0")
define_compiler_support("N3302" "constexpr for <complex>" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3462" "std::result_of and SFINAE" "5.0" "1.0" "19.0" "999")
define_compiler_support("N3469" "constexpr for <chrono>" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3470" "constexpr for <array>" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3471" "constexpr for <initializer_list>, <utility> and <tuple>" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3545" "Improved std::integral_constant" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3642" "User-defined literals for <chrono> and <string>" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3644" "Null forward iterators" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3654" "std::quoted" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3657" "Heterogeneous associative lookup" "5.0" "3.4" "19.0" "999")
#define_compiler_support("N3658" "std::integer_sequence" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3658" "integer_sequence, index_sequence, make_index_sequence etc., to allow tag dispatch on integer packs." "6.3" "5.0" "19.4" "999")
# see https://stackoverflow.com/questions/36222912/is-c17-stdshared-mutex-not-available-yet
define_compiler_support("N3659" "std::shared_timed_mutex" "6.1" "3.4" "19.0" "999")
define_compiler_support("N3668" "std::exchange" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3669" "fixing constexpr member functions without const" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3670" "std::get<T>()" "5.0" "3.4" "19.0" "999")
define_compiler_support("N3671" "Dual-Range std::equal, std::is_permutation, std::mismatch" "5.0" "3.4" "19.0" "999")
# intel support unknwon
define_compiler_support("N3656" "make_unique (Revision 1)" "5.1" "5.0" "19.14" "999")
define_compiler_support("N3857" "std::promise" "5.1" "5.0" "19.14" "999")

#
#   C++11 features
#

message(STATUS "** test ${CMAKE_CXX_COMPILER_ID} compiler ${CMAKE_CXX_COMPILER_VERSION} for C++11 features")
define_compiler_support("N2341" "alignas" "4.8" "3.0" "19.0" "15.0")
define_compiler_support("N2341" "alignof" "4.5" "2.9" "19.0" "15.0")
define_compiler_support("N2427" "Atomic operations" "4.4" "3.1" "17.0" "13.0")
define_compiler_support("N1984" "auto" "4.4" "1.0" "16.0" "12.0")
define_compiler_support("N1653" "C99 preprocessor" "4.3" "1.0" "19.0" "11.1")
define_compiler_support("N2235" "constexpr" "4.6" "3.1" "19.0" "14.1")
define_compiler_support("N2343" "decltype" "4.8" "2.9" "16.0" "12.0")
define_compiler_support("N2346" "Defaulted and deleted functions" "4.4" "3.0" "18.0" "13.1")
define_compiler_support("N1986" "Delegating constructors" "4.7" "3.0" "18.0" "14.0")
define_compiler_support("N2437" "Explicit conversion operators" "4.5" "3.0" "18.0" "13.0")
define_compiler_support("N1791" "Extended friend declarations" "4.7" "2.9" "18.0" "12.0")
define_compiler_support("N1987" "extern template" "3.3" "1.0" "12.0" "9.0")
define_compiler_support("N2764" "Forward enum declarations" "4.6" "3.1" "17.0" "14.0")
define_compiler_support("N2540" "Inheriting constructors" "4.8" "3.3" "19.0" "15.0")
define_compiler_support("N2672" "Initializer lists" "4.4" "3.1" "18.0" "13.0")
define_compiler_support("N2550" "Lambda expressions" "4.5" "3.1" "17.0" "12.0")
define_compiler_support("N2657" "Local and unnamed types as template parameters" "4.5" "2.9" "16.0" "12.0")
define_compiler_support("N1811" "long long" "1.0" "1.0" "14.0" "1.0")
define_compiler_support("N2535" "Inline namespaces" "4.4" "2.9" "19.0" "14.0")
define_compiler_support("N2249" "New character types" "4.4" "2.9" "19.0" "12.1")
define_compiler_support("N2541" "Trailing function return types" "4.4" "2.9" "16.0" "12.0")
define_compiler_support("N2431" "nullptr" "4.6" "2.9" "16.0" "12.1")
define_compiler_support("N2442" "Unicode/Raw string string literals" "4.5" "3.0" "19.0" "11.0")
define_compiler_support("N2765" "User-defined literals" "4.7" "3.1" "19.0" "15.0")
define_compiler_support("N1757" "Right angle brackets" "4.3" "1.0" "14.0" "12.1")
define_compiler_support("N2118" "Rvalue references" "4.3" "2.9" "19.0" "12.0")
define_compiler_support("N2844" "Rvalue references v2.0" "4.3" "2.9" "19.0" "12.0")
define_compiler_support("N1720" "static_assert" "4.3" "2.9" "16.0" "11.0")
define_compiler_support("N2347" "Strongly-typed enum" "4.4" "2.9" "17.0" "13.0")
define_compiler_support("N2258" "Template aliases" "4.7" "3.0" "18.0" "12.1")
define_compiler_support("N2659" "Thread-local storage" "4.8" "3.3" "19.0" "15.0")
define_compiler_support("N2544" "Unrestricted unions" "4.6" "3.0" "19.0" "14.0")
define_compiler_support("N1836" "Type traits" "4.3" "3.0" "14.0" "10.0")
define_compiler_support("N2242" "Variadic templates" "4.4" "2.9" "18.0" "12.1")
define_compiler_support("N2930" "Range-for loop" "4.6" "3.0" "17.0" "13.0")
define_compiler_support("N2761" "Attributes" "4.8" "3.3" "19.0" "12.1")
define_compiler_support("N2439" "ref-qualifiers" "4.8.1" "2.9" "19.0" "14.0")
define_compiler_support("N2756" "Non-static data member initializers" "4.7" "3.0" "18.0" "14.0")
define_compiler_support("N2660" "Dynamic initialization and destruction with concurrency (magic statics)" "4.3" "2.9" "19.0" "11.1")
define_compiler_support("N3050" "noexcept" "4.6" "3.0" "19.0" "13.1.1")
# define_compiler_support("N2670" "Garbage Collection and Reachability-Based Leak Detection" "999" "999" "999" "999")
define_compiler_support("N2670" "Garbage Collection and Reachability-Based Leak Detection (library support)" "6.0" "3.4" "19.0" "999")
define_compiler_support("N2071" "Money, Time, and hexfloat I/O manipulators" "5.0" "3.8" "19.0" "999")
