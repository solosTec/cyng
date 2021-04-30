/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_VALUE_CAST_H
#define CYNG_OBJ_VALUE_CAST_H

#include <cyng/obj/object_cast.hpp>
#ifdef _DEBUG_OBJECT
#include <cyng/io/ostream.h>
#endif

namespace cyng {

	namespace details {

		template <typename T >
		struct cast_policy	{

			using R = std::decay_t<T>;
			static R cast(object const& obj, T const& def) {
				R const* p = object_cast<R>(obj);
				//	if cast failed we return a default value
				return (!p)
					? R{ def }
					: *p
					;			
			}
		};

		/**
		 * @brief detect parameters of type object and return a clone of this object.
		 * 
		 * @tparam T 
		 */
		template <>
		struct cast_policy<object>	{

			using R = object;
			static object cast(object const& obj, object const&) {
				return (!obj)
					? object()
					: obj.clone();
			}
		};

		template <>
		struct cast_policy<char const*>
		{
			using R = std::string;
			static R cast(object const& obj, char const* def) {
				R const* p = object_cast<R>(obj);
				return (!p)
					? R(def)
					: *p
					;
			}
		};

		template <>
		struct cast_policy<char const* const&>
		{
			using R = std::string;
			static R cast(object const& obj, char const* const& def) {
				R const* p = object_cast<R>(obj);
				return (!p)
					? R(def)
					: *p
					;

			}
		};

		template <std::size_t N>
		struct cast_policy<const char(&) [N]>
		{
			using R = std::string;
			static R cast(object const& obj, const char(& def)[N]) {
				R const* p = object_cast<R>(obj);
				return (!p)
					? R(def, N - 1)
					: *p
					;			
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
	auto value_cast(object const& obj, T const& def) -> typename details::cast_policy<T>::R
	{
		using policy = details::cast_policy<T>;
		using type = typename policy::R;
		return policy::cast(obj, def);
	}

	/**
	 * handling C-strings as std::string
	 */
	[[nodiscard]]
	std::string value_cast(object const& obj, char const* p) noexcept;

}

#endif //	CYNG_OBJECT_CAST_HPP
