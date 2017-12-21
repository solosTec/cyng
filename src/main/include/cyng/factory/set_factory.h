/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SET_FACTORY_H
#define CYNG_SET_FACTORY_H


#include <cyng/factory/factory.hpp>
#include <cyng/intrinsics/sets.h>
#include <type_traits>
#include <tuple>
#include <iterator>

namespace cyng 
{	
	/**
	 * Create an attribute
	 */
	template < typename... Args >
	attr_t attr_factory(std::size_t idx, Args&&... args)
	{
		return attr_t(idx, make_object(std::forward<Args>(args)...));
	}
	
	/**
	 * Create a parameter
	 */
	template < typename... Args >
	param_t param_factory(std::string const& name, Args&&... args)
	{
		return param_t(name, make_object(std::forward<Args>(args)...));
	}
	
	/**
	 * Create an object of type attribute 
	 */
	template < typename... Args >
	object set_factory(std::size_t idx, Args&&... args)
	{
		return make_object(attr_factory(idx, std::forward<Args>(args)...));
	}

	/**
	 * Create an object of type parameter 
	 */
	template < typename... Args >
	object set_factory(std::string const& name, Args&&... args)
	{
		return make_object(param_factory(name, std::forward<Args>(args)...));
	}
		
	/**
	 * Convert attributes
	 */
	template <typename T>
	object make_object(std::pair<std::size_t, T>&& v)
	{
 		std::cout << "attr factory\n";
		return factory<attr_t>::create_object(attr_t(v.first, make_object<T>(std::forward<T>(v.second))));
	}
	
	/**
	 * Convert parameters
	 */
	template <typename T>
	object make_object(std::pair<std::string, T>&& v)
	{
//  		std::cout << "param factory\n";
		return factory<param_t>::create_object(param_t(v.first, make_object<T>(std::forward<T>(v.second))));
	}
	
	template <typename T>
 	object make_object(std::pair<const char*, T>&& v)
	{
 		std::cout << "param factory - C\n";
		return factory<param_t>::create_object(param_t(v.first, make_object<T>(std::forward<T>(v.second))));
	}
	
	object tuple_factory();
	object vector_factory();
	object set_factory();
	
	//
	//	tuple builder
	//
	namespace detail
	{
		template < std::size_t IDX, typename C, typename ...Args >
		struct tuple_generator_impl
		{
			using tuple_type = typename std::tuple<Args...>	;
			using value_type = typename std::tuple_element<IDX, tuple_type>::type;
			using iterator_t = typename std::front_insert_iterator<C>;

			static void append(iterator_t bi, std::tuple<Args...> const& inp)
			{
//  				std::cout << '[' << IDX << ']' << std::get< IDX >(inp) << std::endl;
				
				//
				//	append next element
				//
				bi = make_object(std::get< IDX >(inp));

				//	recursion
				tuple_generator_impl<IDX - 1, C, Args...>::append(bi, inp);
			}
		};

		//	terminate recursion 
		template < typename C, typename ...Args >
		struct tuple_generator_impl < 0, C, Args... >
		{
			using tuple_type = typename std::tuple<Args...>	;
			using value_type = typename std::tuple_element<0, tuple_type>::type;
			using iterator_t = typename std::front_insert_iterator<C>;

			static void append(iterator_t bi, std::tuple<Args...> const& inp)
			{
//  				std::cout << '[' << 0 << ']' << std::get< 0 >(inp) << std::endl;
				bi = make_object(std::get< 0 >(inp));
			}
		};

	}	// detail
	
	
	/**
	 * Create an object from each parameter element and add
	 * it to a list of objects (tuple_t)
	 */
	template < typename ...Args >
	tuple_t tuple_factory(Args&&... args)
	{
		using argc = std::integral_constant<std::size_t, sizeof...(Args)>;
		using input_type = typename std::tuple< Args... >;
		using factory_type = detail::tuple_generator_impl< argc::value - 1, tuple_t, Args...>;
		
		//input_type inp(std::forward<Args>(args)...);
		tuple_t tpl;
		//factory_type::append(std::front_inserter(tpl), inp);
		factory_type::append(std::front_inserter(tpl), input_type(std::forward<Args>(args)...));
		
		return tpl;
	}


	template<typename ...Args> 
	tuple_t tuple_factory(std::tuple< Args... >&& inp)
	{
		using argc = std::integral_constant<std::size_t, sizeof...(Args)>;
// 		using input_type = typename std::tuple< Args... >;
		using factory_type = detail::tuple_generator_impl< argc::value - 1, tuple_t, Args...>;
		
		tuple_t tpl;
		factory_type::append(std::front_inserter(tpl), inp);
		
		return tpl;
	}


	/**
	 * Transform a C++ vector into a cyng vector.
	 * Precondition is existing factory method for data type T.
	 */
	template < typename T >
	vector_t vector_factory(std::vector< T > const& data)
	{
		vector_t vec;
		vec.reserve(data.size());
		for (auto const& v : data)
		{
			vec.push_back(make_object(v));
		}
		return vec;
	}
	
	template < typename T >
	vector_t vector_factory(std::initializer_list<T> ilist)
	{
		vector_t vec;
		vec.reserve(ilist.size());
		for (auto const& v : ilist)
		{
			vec.push_back(make_object(v));
		}
		return vec;
	}	
}

#endif 	//	CYNG_SET_FACTORY_H
