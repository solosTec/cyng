/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher 
 * 
 */ 
#ifndef DOCC_OBJ_WRAPPER_HPP
#define DOCC_OBJ_WRAPPER_HPP

#include <cyng/obj/core/object_interface.h>
#include <cyng/obj/core/type.hpp>
#include <cyng/obj/tracker.h>
#include <cyng/obj/object.h>

#include <utility>
#include <functional>
#include <type_traits>

namespace docscript {

	class object;

	template<typename T >
	class wrapper : public obj_interface
	{
		template < typename U >
		friend const U* object_cast(object const&) noexcept;
		template < typename U >
		friend auto object_release(object& obj) noexcept -> std::pair<U*, obj_interface*>;


		using this_t	= wrapper< T >;

		static_assert(std::is_copy_assignable<T>::value, "type is not copy-assignable");

	public:
		wrapper(wrapper&& w)
			: value_(std::move(w.value_))
		{
			object::tracker_.inc();
		}

		template < typename U >
		wrapper(U&& v)
			: value_(std::move(v))
		{
			object::tracker_.inc();
		}

		template < typename... Args >
		wrapper(Args&&... args)
			: value_(std::forward<Args>(args)...)
		{
			object::tracker_.inc();
		}

		virtual ~wrapper()
		{}

		[[nodiscard]] 
		virtual obj_interface* clone() const override
		{
			//
			//	T must be copy-able
			//
			T clone = value_;
			return new wrapper<T>(std::move(clone));
		}

		virtual std::size_t hash() const noexcept override
		{
			return std::hash<T>{}(value_);
		}

		virtual type_interface const& rtti() const noexcept override
		{
			return type_;
		}


	private:
		/**
		 * The hidden value
		 */
		T value_;

		/**
		 * Type info is static. Therefore all pointers to the same
		 * class info are equal.
		 */
		static const type<T> type_;

	};

	/**
	 * Initialize static data member type_
	 */
	template <typename T>
	type<T> const /*typename*/ wrapper<T>::type_;


	/**
	 * Specialized for arrays
	 *
	 * @tparam T hidden data type
	 * @tparam N array size
	 */
	template <typename T, std::size_t N>
	class wrapper<T[N]> : public obj_interface
	{
		using indices = typename std::make_index_sequence<N>;
		using this_t = wrapper< T[N] >;

	public:
		/**
		 * Only the default constructor can be called when creating objects in an array.
		 */
		wrapper(T const(&v)[N])
			: wrapper(v, indices{})
		{
			object::tracker_.inc();
		}

		template<std::size_t... I>
		wrapper(T const(&v)[N], std::index_sequence<I...>)
			: value_{ v[I]... }
		{
			object::tracker_.inc();
		}

		virtual ~wrapper()
		{}

		[[nodiscard]] 
		virtual obj_interface* clone() const override
		{
			//
			//	Object must be copy-able
			//	
			return new wrapper<T[N]>(value_);
		}

		virtual std::size_t hash() const noexcept override
		{
			std::size_t h{ 0 };
			auto f = std::hash<T>{};
			for (auto const& v : value_) {

				//
				//	combine all values
				//
				h ^= f(v) << 1;
			}
			return h;
		}

		virtual type_interface const& rtti() const noexcept override
		{
			return type_;
		}

	private:
		/**
		 * The hidden array
		 */
		T value_[N];

		/**
		 * Type info is static. Therefore all pointers to the same
		 * class info are equal.
		 */
		static const type<T[N]> type_;

	};

	/**
	 * Initialize static data member class_
	 */
	template <typename T, std::size_t N>
	type<T[N]> const wrapper<T[N]>::type_;

}

#endif
