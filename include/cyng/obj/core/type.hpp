/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_OBJ_TYPE_HPP
#define DOCC_OBJ_TYPE_HPP

#include <cyng/obj/type_interface.h>
#include <cyng/obj/tag.hpp>

#include <typeindex>
#include <type_traits>

namespace docscript {


	/**
	 * Provide the class interface of the specified data type.
	 * Thus providing compile time information at runtime.
	 */
	template <typename T>
	class type : public type_interface
	{
	public:
		type() = default;

		/**	@brief  simple C++ runtime type identification (RTTI).
		 *
		 * Make sure RTTI is enabled
		 *
		 *  @return runtime type identification (RTTI) provided by C++ compiler
		 */
		virtual const std::type_info& rtti() const override
		{
			return typeid (T);
		}

		/**
		 * This is the result of the sizeof() operator.
		 *
		 * @return Storage size in bytes
		 */
		virtual std::size_t size() const override
		{
			return sizeof (T);
		}

		/**
		 * @return Dimension of the array
		 */
		virtual std::size_t extend() const override
		{
			return std::extent<T>::value;
		}

		/**
		 * Checks whether T is an integral type.
		 */
		virtual bool is_integral() const override
		{
			return std::is_integral<T>::value;
		}

		/**
		 * Checks whether T is a floating-point type.
		 */
		virtual bool is_floating_point() const override
		{
			return std::is_floating_point<T>::value;
		}

		/**
		 * Checks whether T is  an enumeration type.
		 */
		virtual bool is_enum() const override
		{
			return std::is_enum<T>::value;
		}

		/**
		 * checks if a type is const-qualified.
		 */
		virtual bool is_const() const override
		{
			return std::is_const<T>::value;
		}

		/**
		 * This is the position in the global type tuple.
		 *
		 * @return The internal type tag (type_code)
		 */
		virtual std::size_t tag() const
		{
			return built_in_type<T>()
				? type_tag_traits<T>()
				: -1
				;
		}

		/**
		 * @return the type if for built-in types
		 */
		virtual const char* type_name() const
		{
			return intrinsic_name<T>();
		}

		/**
		 * Test if both objects of same type
		 */
		template <typename U>
		constexpr bool is_same() const
		{
			return typeid(T) == typeid (U);
		}

	};

}
#endif
