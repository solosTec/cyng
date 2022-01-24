/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_OBJ_TYPE_INTERFACE_H
#define CYNG_OBJ_TYPE_INTERFACE_H

#include <cstdint>
#include <cstddef>
#include <typeinfo>

namespace cyng {

	class type_interface
	{
	public:
		/**	@brief  simple C++ runtime type identification (RTTI).
		 *
		 * Make sure RTTI is enabled
		 *
		 *  @return runtime type identification (RTTI) provided by C++ compiler
		 */
		virtual const std::type_info& rtti() const = 0;

		/**
		 * This is the result of the sizeof() operator.
		 *
		 * @return Storage size in bytes
		 */
		virtual std::size_t size() const noexcept = 0;

		/**
		 * @return Dimension of the array
		 */
		virtual std::size_t extend() const noexcept = 0;

		/**
		 * Checks whether T is an integral type.
		 */
		virtual bool is_integral() const noexcept = 0;

		/**
		 * Checks whether T is a floating-point type.
		 */
		virtual bool is_floating_point() const noexcept = 0;

		/**
		 * Checks whether T is an enumeration type.
		 */
		virtual bool is_enum() const noexcept = 0;

		/**
		 * Checks whether T is const-qualified
		 */
		virtual bool is_const() const noexcept = 0;

		/**
		 * @return true if build-in type
		 */
		virtual bool is_intrinsic() const noexcept = 0;

		/**
		 * This is the position in the global type tuple.
		 * 
		 * @return The internal type tag (type_code)
		 */
		virtual std::uint16_t tag() const noexcept = 0;

		/**
		 * @return true if type is an instrinsic container class
		 */
		virtual bool is_intrinsic_container() const noexcept = 0;

		/**
		 * @return the type if for built-in types
		 */
		virtual const char* type_name() const = 0;

	};
}
#endif
