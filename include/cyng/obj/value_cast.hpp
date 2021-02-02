/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_VALUE_CAST_H
#define DOCC_OBJ_VALUE_CAST_H

#include <cyng/obj/object_cast.hpp>
#ifdef _DEBUG_OBJECT
#include <cyng/io/ostream.h>
#endif

namespace docscript {

	namespace {

		template <typename T >
		struct cast_policy	{

			static T cast(object const& obj, T const& def) {
				T const* p = object_cast<T>(obj);
				//	if cast failed we return a default value
				return (!p)
					? def
					: *p
					;			
			}
		};

		/**
		 * @brief detect parameters of tyüe object and return a clone of this object.
		 * 
		 * @tparam T 
		 */
		template <>
		struct cast_policy<object>	{

			static object cast(object const& obj, object const&) {
				return (!obj)
					? object()
					: obj.clone();
			}
		};
	}

	/**
	 * If the hidden value is not of type T the function returns a nullptr.
	 * This is the foundation for all other casts.
	 *
	 * @tparam T cast object 
	 * @return constant pointer to the value hosted by the object.
	 */
	template < typename T >
	[[nodiscard]]
	auto value_cast(object const& obj, T const& def) -> std::decay_t<T>  
	{
		using type = std::decay_t<T>;
		using policy = cast_policy<type>;
		return policy::cast(obj, def);
	}

	/**
	 * handling C-strings as std::string
	 */
	[[nodiscard]]
	std::string value_cast(object const& obj, char const* p) noexcept;

}

#endif //	CYNG_OBJECT_CAST_HPP
