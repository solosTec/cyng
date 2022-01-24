/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_TYPE_HPP
#define CYNG_OBJ_TYPE_HPP

#include <cyng/obj/type_interface.h>
#include <cyng/obj/tag.hpp>

#include <typeindex>
#include <type_traits>

namespace cyng {


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
		virtual std::size_t size() const noexcept override
		{
			return sizeof (T);
		}

		/**
		 * @return Dimension of the array
		 */
		virtual std::size_t extend() const noexcept override
		{
			return std::extent<T>::value;
		}

		/**
		 * Checks whether T is an integral type.
		 */
		virtual bool is_integral() const noexcept override
		{
			return std::is_integral<T>::value;
		}

		/**
		 * Checks whether T is a floating-point type.
		 */
		virtual bool is_floating_point() const noexcept override
		{
			return std::is_floating_point<T>::value;
		}

		/**
		 * Checks whether T is  an enumeration type.
		 */
		virtual bool is_enum() const noexcept override
		{
			return std::is_enum<T>::value;
		}

		/**
		 * checks if a type is const-qualified.
		 */
		virtual bool is_const() const noexcept override
		{
			return std::is_const<T>::value;
		}

		/**
		 * @return true if build-in type
		 */
		virtual bool is_intrinsic() const noexcept override {
			return built_in_type<T>();
		}

		/**
		 * This is the position in the global type tuple.
		 *
		 * @return The internal type tag (type_code)
		 */
		virtual std::uint16_t tag() const noexcept override {
			return built_in_type<T>()
				? type_tag_traits<T>()
				: TC_EXTRINSIC
				;
		}

		virtual bool is_intrinsic_container() const noexcept override {
			return built_in_type<T>()
				? is_container(get_type_code<T>())
				: false
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
			return std::is_same_v<T, U>;
		}

	};

}
#endif
