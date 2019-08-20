/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_FACTORY_HPP
#define CYNG_FACTORY_HPP

#include <cyng/object.h>
#include <cyng/core/wrapper.hpp>
#include <cyng/intrinsics/null.h>
#include <iostream>
#include <initializer_list>
#include <boost/core/demangle.hpp>

namespace cyng 
{
	/**
	 * @tparam T hidden object type 
	 */
	template <typename T>
	struct factory
	{
		using wrapper_t = core::wrapper< T >;
		
		/**
		 * Generic object factory. Generates one or multiple objects.
		 */
		template <typename... Args>
		static core::shared_object create_so(Args&&... args)
		{
			return std::static_pointer_cast< core::object_interface >(std::make_shared< wrapper_t >(std::forward<Args>(args)...));
		}
		
		/**
		 * Generic object factory. Generates one or multiple objects.
		 */
		template <typename... Args>
		static object create_object(Args&&... args)
		{
			return object(create_so(std::forward<Args>(args)...));
		}
				
		/**
		 * Generic object factory, that allows to specify a user defined deleter.
		 */
// 		template < typename D /*= deleter_t*/, typename... Args>
// 		static object create_array(/*D d, */Args&&... args)
// 		{
// 			return object(std::static_pointer_cast< core::object_interface >(std::shared_ptr< wrapper_t > ( new wrapper_t(std::forward<Args>(args)...), D())));
// 		}
	};
	
	namespace detail 
	{
		template <typename T>
		struct factory_policy
		{
			using value_type = typename std::decay< T >::type;
			
			//
			//	boxing
			//
			static object boxing(T&& v)
			{
				return factory<value_type>::create_object(std::forward<T>(v));
			}
			static object boxing(T const& v)
			{
				return factory<value_type>::create_object(v);
			}
		};
		
		/**
		 * Use the value to which it points, not the pointer.
		 */
		template <typename T>
		struct factory_policy<T*>
		{
			using value_type = typename std::decay< T >::type;
			static object boxing(T* p)
			{
				//std::cout << "--- pointer " 
				//<< boost::core::demangle(typeid( T ).name())
				//<< " ---\n";
				return factory<value_type>::create_object(*p);
			}
		};

		/**
		 * Prevent nested objects.
		 */
		template <>
		struct factory_policy<object>
		{
			static object boxing(object&& obj)
			{
// 	 			std::cout << "--- prevent nested objects&& ---\n";
				return obj;
			}
			static object boxing(object const& obj)
			{
				//std::cout << "--- prevent nested objects const& ---\n";
				return obj;
			}
		};

		/**
		 * Handle C-style strings as std::string.
		 * Since std::decay converts <char[N]> to <char *> 
		 * this specialization should no longer be usefull.
		 * See specialization for <char const*>
		 */
 		template <std::size_t N>
		struct factory_policy<const char(&)[N]>
		{
			static object boxing(char const(&p)[N])
			{
				return factory<std::string>::create_object(std::string(p, N - 1));
			}
		};
		
		template <>
		struct factory_policy<char const*>
		{
			static object boxing(char const* p)
			{
				return factory<std::string>::create_object(std::string(p));
			}
		};
	}
	
	/**
	 * Generic factory method. 
	 * The compiler picks this function if specified target type T
	 * and source type(s) Args... are different.
	 */
	template <typename T, typename... Args>
	object make_object(Args&&... args)
	{
//  		std::cout << "variadic factory\n";
		using value_type = typename std::decay< T >::type;
		return factory<value_type>::create_object(std::forward<Args>(args)...);
	}
	
	/** 
	 * Initializer list generates an object of type vector_t.
	 *
	 * Example
	 * @code
	 auto obj = make_object({ 1, 2, 3 });
	 * @endcode
	 * 
	 * @see vector_factory(std::initializer_list<T>)
	 */
	template <typename T>
	object make_object(std::initializer_list<T> list)
	{
		vector_t vec;
		vec.reserve(list.size());
		std::transform(list.begin(), list.end(), std::back_inserter(vec), [](T const& v) {
			return make_object<T>(v);
		});
		return make_object<vector_t>(std::move(vec));
	}

	/**
	 * Same target and source type.
	 * 
	 * @code
	 * factory<T>::create_object(T&&);
	 * @endcode
	 */
	template <typename T>
	object make_object(T&& v)
	{
		using value_type = typename std::decay< T >::type;
		return detail::factory_policy<value_type>::boxing(std::forward<T>(v));
	}
		
	/**
	 * produce the null object 
	 */
	inline object make_object(void)
	{
		return make_object(null());
	}
}

#endif 	//	CYNG_FACTORY_HPP
