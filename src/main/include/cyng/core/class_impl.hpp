/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_CLASS_IMPL_HPP
#define CYNG_CLASS_IMPL_HPP

#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag_names.hpp>
#include <cyng/traits.h>
// #include <cyng/intrinsics/traits/tag.hpp>
#include <typeinfo>
#include <typeindex>
#include <type_traits>
#ifdef __GNUC__
#include <features.h>
#endif

namespace cyng 
{
	namespace core 
	{
		/**
		 * Provide the class interface of the specified data type. 
		 * Thus providing compile time information at runtime.
		 */
		template <typename T>
		struct class_impl : class_interface
		{
			/**	@brief general type info
			 * 
			 * @return C++ runtime type_info object
			 * @remark virtual keyword is not required but clarifies the intention to override
			 * methods of base class object_interface.
			 */
			virtual const std::type_info & type() const override
			{
				return typeid(T);
			}
			
			/**
			 * @return The hash code of the associated data type without
			 * any qualifiers.
			 */
			virtual std::size_t hash() const override
			{
				return std::type_index(typeid(typename std::decay<T>::type)).hash_code();
			}
			
			/**
			 * @return True if data type has built-in support 
			 */
			virtual bool is_built_in() const override
			{
				return built_in_type<T>();
			}
			
			virtual std::size_t tag() const override 
			{
				return built_in_type<T>()
				? type_tag_traits<T>()
				: cyng::traits::type_tag<T>::tag::value
				;
			}
			
			virtual const char* type_name() const override
			{
                //  linkage errors with gcc 6.3 (ubuntu 17.04)
                //  breaks some unit tests
#ifdef __GNUC__
#if __GNUC_PREREQ(6,4)
                return cyng::traits::get_tag_name<T>();
#else
                return "?";
#endif
#else
                return cyng::traits::get_tag_name<T>();
#endif
			}

			/** @brief Size in bytes allocated on stack.
			 * 
			 * @return Storage size in bytes. 
			 */
			std::size_t size() const override
			{
				return sizeof(T);
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
			 * Checks whether T is an array type. 
			 */
			virtual bool is_array() const override
			{
				return std::is_array<T>::value;
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
		};
	}

	/**
	 * convinience type test
	 */
	template <type_code TC>
	bool is_of_type(object const& obj) {
		return obj.get_class().tag() == TC;
	}

}

#endif 	//	CYNG_CLASS_IMPL_HPP

