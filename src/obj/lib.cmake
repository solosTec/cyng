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
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/container_factory.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/factory.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/object.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/tag.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/tracker.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/type_interface.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/util.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/compare.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/clone.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics.h
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
    src/intrinsics/date.cpp
    src/intrinsics/time.cpp

    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/aes_key.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/buffer.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/container.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/digest.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/eod.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/mac.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/pid.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/obis.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/edis.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/null.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/op.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/severity.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/version.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/color.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/raw.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/program.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/date.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/intrinsics/time.h
)

set (object_core 
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/core/object_interface.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/core/type.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/core/wrapper.hpp
)

set (object_cast 
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/function_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/numeric_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/object_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/tuple_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/value_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/vector_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/set_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/container_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/array_cast.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/buffer_cast.hpp
    src/value_cast.cpp
    src/buffer_cast.cpp
    src/container_cast.cpp
)

set (object_algorithm 
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/find.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/size.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/reader.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/dom_walker.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/dom_transform.h
#    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/dom_extract.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/merge.h
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/add.hpp
    ${PROJECT_SOURCE_DIR}/../../include/cyng/obj/algorithm/swap_bytes.h
    src/find.cpp
    src/size.cpp
    src/reader.cpp
    src/dom_walker.cpp
    src/dom_transform.cpp
#    src/dom_extract.cpp
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

