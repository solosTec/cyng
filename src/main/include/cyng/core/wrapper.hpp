/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_CORE_WRAPPER_HPP
#define CYNG_CORE_WRAPPER_HPP

#include <CYNG_project_info.h>
#include <cyng/compatibility/legacy_mode.hpp>
#include <cyng/core/object_interface.h>
#include <cyng/object.h>
#include <cyng/core/class_impl.hpp>
#include <cyng/core/object_cast_fwd.h>
#include <cyng/core/value_cast_fwd.h>
#include <cyng/intrinsics/policy/hash.h>
#include <iostream>
#include <utility>
#include <functional>

namespace cyng 
{
	namespace core 
	{
		/**
		 * @tparam T hidden data type 
		 */
		template <typename T>
		class wrapper 
		: public object_interface
		, public std::enable_shared_from_this< wrapper< T > >
		{
			template < typename U >
			friend const U* cyng::object_cast(object const&) noexcept;
			
			template < typename U >
			friend U cyng::value_cast(object const&, U const&) noexcept;
// 			friend const U& cyng::value_cast(object const&, U const&) noexcept;
					
			using class_t = class_impl< T >;
			
			using this_type = wrapper< T >;
		
		public:
			
			template < typename... Args >
			wrapper(Args&&... args)
				: held_(std::forward<Args>(args)...)
			{}

			wrapper(wrapper const&) = delete;
			wrapper& operator=(wrapper const&) = delete;

			virtual ~wrapper()
			{}
			
			/**
			 * Offers a route for safe copying. Requires that T 
			 * can be copied.
			 * 
			 * @return a copy of the value as shared pointer 
			 */
			virtual shared_object clone() const override
			{
				return shared_object();
 				//return std::make_shared< wrapper< T > >(held_);
			}
			
			virtual class_interface const& get_class() const noexcept override
			{
				return static_cast<class_interface const&>(class_);
			}
			
			virtual std::size_t hash() const override
 			{
				return std::hash<T>()(held_);
 			}
 			
			/**
			 * Reiteration of std::equal_to
			 */
 			virtual bool equal_to(object const& obj) const noexcept override
 			{
				if (typeid(T) == obj.get_class().type())
				{
					auto wp = std::dynamic_pointer_cast< this_type >(obj.value_);
					if (wp)
					{
						return std::equal_to<T>()(this->held_, wp->held_);
					}
					return true;
				}
				return false;
			}
 			
			
			/**
			 * Reiteration of std::less
			 */
			virtual bool less(object const& obj) const noexcept override
			{
				if (typeid(T) == obj.get_class().type())
				{
					auto wp = std::dynamic_pointer_cast< this_type >(obj.value_);
					if (wp)
					{
						return std::less<T>()(this->held_, wp->held_);
					}
					return true;
				}
				return false;
			}

		private:
			/**
			 * The hidden value 
			 */
			T held_;
			
			/*
			 * Class info is static. Therefore all pointers to the same
			 * class info are equal.
			 */
			static const class_t class_;
		};
		
		/**
		 * Initialize static data member class_
		 */
		template <typename T>
		typename wrapper<T>::class_t const wrapper<T>::class_;
		
		/**
		 * Specialized for arrays
		 * 
		 * @tparam T hidden data type 
		 * @tparam N array size 
		 */
		template <typename T, std::size_t N>
		class wrapper<T[N]> 
		: public object_interface
		, public std::enable_shared_from_this< wrapper< T[N] > >
		{
		template < typename U >
		friend const U* cyng::object_cast(object const&) noexcept;
		
		template < typename U >
// 		friend const U& cyng::value_cast(object const&, U const&) noexcept;
		friend U cyng::value_cast(object const&, U const&) noexcept;

		using class_t = class_impl< T[N] >;
		
#if defined(CYNG_LEGACY_MODE_ON)
// 		using indices = typename generate_sequence<N>::type;
		using indices = meta::make_index_sequence<N>;
#else
		using indices = typename std::make_index_sequence<N>;
#endif

		public:
			wrapper()
			: held_{0}
			{}

			/**
			 * Delegate constructor
			 */
			wrapper(T const(&v)[N])
			: wrapper(v, indices{})
			{}
			
#if defined(CYNG_LEGACY_MODE_ON)
			template<std::size_t... I>
			wrapper(T const(&v)[N], typename meta::generate_index_sequence<N>::type)
			: held_{ v[I]... }
			{}
#else
			template<std::size_t... I>
			wrapper(T const(&v)[N], std::index_sequence<I...>)
			: held_{ v[I]... }
			{}
#endif

			virtual ~wrapper()
			{}
						
			virtual shared_object clone() const override
			{
 				return std::make_shared< wrapper< T[N] > >(held_);
			}
			
			virtual class_interface const& get_class() const noexcept override
			{
 				return static_cast<class_interface const&>(class_);
			}
			
 			virtual std::size_t hash() const override
 			{
				//	ToDo: implement for arrays
				return 0;
			}
			
 			virtual bool equal_to(object const& obj) const noexcept override
 			{
				//	ToDo: implement for arrays
				if (obj.get_class().is_array()) {

				}
				return false;
			}

			virtual bool less(object const& obj) const noexcept override
			{
				//	ToDo: implement for arrays
				if (obj.get_class().is_array()) {

				}
				return false;
			}

		private:
			/**
			 * The hidden array 
			 */
			T held_[N];
			
			/*
			 * Class info is static. Therefore all pointers to the same
			 * class info are equal.
			 */
			static const class_t class_;
			
		};
		
		/**
		 * Initialize static data member class_
		 */
		template <typename T, std::size_t N>
		typename wrapper<T[N]>::class_t const wrapper<T[N]>::class_;
		
	}
}

#endif 	//	CYNG_CORE_WRAPPER_HPP

