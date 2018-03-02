# 
#	reset 
#
set (core_lib)

set (core_cpp
	lib/core/src/core.cpp
	lib/core/src/object.cpp
	lib/core/src/tracker.cpp
	lib/core/src/reference.cpp
 	lib/core/src/type.cpp
 	lib/core/src/chrono.cpp
  	
  	lib/core/src/tree_walker.cpp
  	lib/core/src/reader.cpp
  	lib/core/src/set_cast.cpp
  	lib/core/src/algorithm.cpp
  	lib/core/src/split.cpp
)
    
set (core_h
    src/main/include/cyng/cyng.h
    src/main/include/cyng/object.h
    src/main/include/cyng/type.h
    src/main/include/cyng/core/class_interface.h
    src/main/include/cyng/core/class_impl.hpp
    src/main/include/cyng/core/object_interface.h
    src/main/include/cyng/core/object_cast_fwd.h
    src/main/include/cyng/object_cast.hpp
    src/main/include/cyng/core/value_cast_fwd.h
    src/main/include/cyng/value_cast.hpp
    src/main/include/cyng/vector_cast.hpp
    src/main/include/cyng/set_cast.h
    src/main/include/cyng/tuple_cast.hpp
    src/main/include/cyng/core/wrapper_fwd.h
    src/main/include/cyng/core/wrapper.hpp
	src/main/include/cyng/tracker.h
	src/main/include/cyng/reference.h
#     src/main/include/cyng/core/deleter.hpp
    src/main/include/cyng/chrono.h
    
    src/main/include/cyng/dom/tree_walker.h
    src/main/include/cyng/dom/reader.h
    src/main/include/cyng/dom/algorithm.h

    src/main/include/cyng/compatibility/general.h
    src/main/include/cyng/util/slice.hpp    
    src/main/include/cyng/util/split.h
)

if (${PROJECT_NAME}_LEGACY_MODE EQUAL ${PROJECT_NAME}_LEGACY_MODE_ON)
set (core_compatibility
    src/main/include/cyng/compatibility/legacy_mode.hpp	
)
endif()	

set (core_factory
    src/main/include/cyng/factory.h
    src/main/include/cyng/factory/factory_fwd.hpp
    src/main/include/cyng/factory/factory.hpp
    src/main/include/cyng/factory/set_factory.h
    src/main/include/cyng/factory/chrono_factory.h

  	lib/core/src/intrinsics/set_factory.cpp
  	lib/core/src/intrinsics/chrono_factory.cpp
)

set (core_intrinsics
    src/main/include/cyng/intrinsics.h
    src/main/include/cyng/intrinsics/null.h
    src/main/include/cyng/intrinsics/eod.h
    src/main/include/cyng/intrinsics/sets.h
    src/main/include/cyng/intrinsics/version.h
 	src/main/include/cyng/intrinsics/op.h
 	src/main/include/cyng/intrinsics/label.h
 	src/main/include/cyng/intrinsics/buffer.h
 	src/main/include/cyng/intrinsics/mac.h
 	src/main/include/cyng/intrinsics/lockable.h
	src/main/include/cyng/intrinsics/digest.h
	src/main/include/cyng/intrinsics/color.hpp

 	lib/core/src/intrinsics/sets.cpp
 	lib/core/src/intrinsics/version.cpp
  	lib/core/src/intrinsics/op.cpp
  	lib/core/src/intrinsics/label.cpp
  	lib/core/src/intrinsics/mac.cpp
  	lib/core/src/intrinsics/lockable.cpp
  	lib/core/src/intrinsics/digest.cpp
  	
  	lib/core/src/intrinsics/tag_names.cpp
  	lib/core/src/intrinsics/compare.cpp
  	lib/core/src/intrinsics/hash.cpp
  	lib/core/src/intrinsics/traits.cpp

#    src/main/include/cyng/compare.h
    src/main/include/cyng/traits.h
    src/main/include/cyng/intrinsics/policy/compare.hpp
    src/main/include/cyng/intrinsics/policy/hash.h
    src/main/include/cyng/intrinsics/traits/tag.hpp
    src/main/include/cyng/intrinsics/traits/tag_names.hpp    
    src/main/include/cyng/intrinsics/traits.hpp
)

source_group("factory" FILES ${core_factory})
source_group("intrinsics" FILES ${core_intrinsics})

# define the core lib
set (core_lib
  ${core_cpp}
  ${core_h}
  ${core_compatibility}
  ${core_factory}
  ${core_intrinsics}
)
