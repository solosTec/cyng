/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_CLASS_INTERFACE_H
#define CYNG_CLASS_INTERFACE_H


#include <cstdint>
#include <memory>

namespace cyng 
{
	namespace core 
	{
		/**
		 * Provide compile time meta data at runtime.
		 */
		class class_interface
		{
		public:
			//	general type info

			/**	@brief  simple C++ runtime type identification (RTTI).
			 * 
			 * Make sure RTTI is enabled
			 *
			 *  @return runtime type identification (RTTI) provided by C++
			 * compiler
			 */
			virtual const std::type_info & type() const = 0;
			
			/**
			 * @return The hash code of the associated data type (not to be confused
			 * with the value hash).
			 */
			virtual std::size_t hash() const = 0;
			
			/**
			 * @return True if data type has built-in support 
			 */
			virtual bool is_built_in() const = 0;

			/**
			 * @return The internal type tag (type_code)
			 */
 			virtual std::size_t tag() const = 0;
			
			/**
			 * @return the type if for built-in types
			 */
			virtual const char* type_name() const = 0;
			
			/**
			 * This is the result of the sizeof() operator.
			 * 
			 * @return Storage size in bytes
			 */
			virtual std::size_t size() const = 0;

			/**
			 * @return Dimension of the array
			 */
			virtual std::size_t extend() const = 0;

			/**
			 * Checks whether T is an integral type. 
			 */
			virtual bool is_integral() const = 0;
			
			/**
			 * Checks whether T is a floating-point type. 
			 */
			virtual bool is_floating_point() const = 0;
			
			/**
			 * Checks whether T is an array type. 
			 */
			virtual bool is_array() const = 0;

			/**
			 * Checks whether T is an enumeration type. 
			 */
			virtual bool is_enum() const = 0;
			
			/**
			 * Checks whether T is const-qualified
			 */
			virtual bool is_const() const = 0;
			
		};
		
		/**
		 * Test for built-in data type 
		 */
		inline bool operator==(class_interface const& ci, std::size_t tag)
		{
			return ci.tag() == tag;
		}
	}
}

#endif 	//	CYNG_OBJECT_INTERFACE_H

