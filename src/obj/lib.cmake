# 
#	reset 
#
set (object_lib)

set (object_cpp
    src/obj/object.cpp
    src/obj/tracker.cpp
    src/obj/factory.cpp
    src/obj/value_cast.cpp
    src/obj/buffer_cast.cpp
    src/obj/container_cast.cpp
    src/obj/util.cpp
    src/obj/tag.cpp
    src/obj/container_factory.cpp
)
    
set (object_h
    include/cyng/obj/buffer_cast.hpp
    include/cyng/obj/container_factory.hpp
    include/cyng/obj/factory.hpp
    include/cyng/obj/object.h
    include/cyng/obj/tag.hpp
    include/cyng/obj/tracker.h
    include/cyng/obj/type_interface.h
    include/cyng/obj/util.hpp
    include/cyng/obj/compare.hpp
)

set (object_intrinscis
    src/obj/intrinsics/buffer.cpp
    src/obj/intrinsics/version.cpp
    src/obj/intrinsics/mac.cpp
    src/obj/intrinsics/obis.cpp
    src/obj/intrinsics/edis.cpp
    src/obj/intrinsics/pid.cpp
    src/obj/intrinsics/container.cpp

    include/cyng/obj/intrinsics/aes_key.hpp
    include/cyng/obj/intrinsics/buffer.h
    include/cyng/obj/intrinsics/container.h
    include/cyng/obj/intrinsics/digest.hpp
    include/cyng/obj/intrinsics/eod.h
    include/cyng/obj/intrinsics/mac.h
    include/cyng/obj/intrinsics/pid.h
    include/cyng/obj/intrinsics/obis.h
    include/cyng/obj/intrinsics/edis.h
    include/cyng/obj/intrinsics/null.h
    include/cyng/obj/intrinsics/op.h
    include/cyng/obj/intrinsics/severity.h
    include/cyng/obj/intrinsics/version.h
)

set (object_core 
    include/cyng/obj/core/object_interface.h
    include/cyng/obj/core/type.hpp
    include/cyng/obj/core/wrapper.hpp
)

set (object_cast 
    include/cyng/obj/function_cast.hpp
    include/cyng/obj/numeric_cast.hpp
    include/cyng/obj/object_cast.hpp
    include/cyng/obj/tuple_cast.hpp
    include/cyng/obj/value_cast.hpp
    include/cyng/obj/vector_cast.hpp
    include/cyng/obj/container_cast.hpp
)

set (object_algorithm 
    include/cyng/obj/algorithm/find.h
    include/cyng/obj/algorithm/reader.hpp
    include/cyng/obj/algorithm/dom_walker.h
    src/obj/find.cpp
    src/obj/reader.cpp
    src/obj/dom_walker.cpp
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

