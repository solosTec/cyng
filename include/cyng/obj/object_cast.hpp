/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_OBJECT_CAST_H
#define CYNG_OBJ_OBJECT_CAST_H

#include <cyng/obj/core/wrapper.hpp>
#include <cyng/obj/object.h>
#include <cyng/obj/tracker.h>

namespace cyng {

	/**
	 * If the hidden value is not of type T the function returns a nullptr.
	 * This is the foundation for all other casts.
	 *
	 * @tparam T cast object 
	 * @return constant pointer to the value hosted by the object.
	 */
	template < typename T >
	T const* object_cast(object const& obj) noexcept
	{
		using value_type = wrapper< T >;

		//	cast to the correct pointer
		auto const p = dynamic_cast< value_type* >(obj.obj_.get());
		
		//	if cast failed we return a null pointer
		return (!p)
			? nullptr
			: &p->value_
			;
	}	

	/**
	 * be carefull and don't change the underlying data type
	 * only the value.
	 */
	template < typename T >
	T* object_cast(object& obj) noexcept
	{
		using value_type = wrapper< T >;

		//	cast to the correct pointer
		auto p = dynamic_cast<value_type*>(obj.obj_.get());

		//	if cast failed we return a null pointer
		return (!p)
			? nullptr
			: &p->value_
			;
	}

	/**
	 * The object releases the pointer.
	 * Note: Don't forget to delete the pointer
	 *
	 * @return pointer to the wrapper object
	 */
	template < typename T >
	[[nodiscard]]
	auto object_release(object& obj) noexcept -> std::pair<T*, obj_interface*>
	{
		using value_type = wrapper< T >;

		//	release ptr
		auto ptr = obj.obj_.release();

		//	cast to the correct pointer
		auto p = dynamic_cast<value_type*>(ptr);

		//	if cast failed we return a null pointer
		if (p == nullptr) {

			//	reset
			obj.obj_.reset(ptr);
			return std::make_pair(nullptr, nullptr);
		}
		return std::make_pair(&p->value_, ptr);
	}

}

#endif //	CYNG_OBJECT_CAST_HPP
