# 
#	reset 
#
set (object_lib)

set (object_cpp
    src/object.cpp
    src/tracker.cpp
    src/factory.cpp
    src/util.cpp
    src/tag.cpp
    src/container_factory.cpp
)
    
set (object_h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/container_factory.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/factory.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/object.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/tag.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/tracker.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/type_interface.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/util.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/compare.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/clone.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics.h
)

set (object_intrinscis
    src/intrinsics/buffer.cpp
    src/intrinsics/version.cpp
    src/intrinsics/mac.cpp
    src/intrinsics/obis.cpp
    src/intrinsics/edis.cpp
    src/intrinsics/pid.cpp
    src/intrinsics/container.cpp
    src/intrinsics/raw.cpp
    src/intrinsics/program.cpp

    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/aes_key.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/buffer.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/container.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/digest.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/eod.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/mac.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/pid.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/obis.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/edis.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/null.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/op.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/severity.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/version.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/color.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/raw.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/intrinsics/program.h
)

set (object_core 
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/core/object_interface.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/core/type.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/core/wrapper.hpp
)

set (object_cast 
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/function_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/numeric_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/object_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/tuple_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/value_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/vector_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/set_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/container_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/array_cast.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/buffer_cast.hpp
    src/value_cast.cpp
    src/buffer_cast.cpp
    src/container_cast.cpp
)

set (object_algorithm 
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/algorithm/find.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/algorithm/size.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/algorithm/reader.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/algorithm/dom_walker.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/algorithm/merge.h
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/algorithm/add.hpp
    ${CMAKE_SOURCE_DIR}/include/cyng/obj/algorithm/swap_bytes.h
    src/find.cpp
    src/size.cpp
    src/reader.cpp
    src/dom_walker.cpp
    src/merge.cpp
)


source_group("intrinscis" FILES ${object_intrinscis})
source_group("core" FILES ${object_core})
source_group("cast" FILES ${object_cast})
source_group("algorithm" FILES ${object_algorithm})

# define the docscript lib
set (object_lib
  ${object_cpp}
  ${object_h}
  ${object_intrinscis}
  ${object_core}
  ${object_cast}
  ${object_algorithm}
)

