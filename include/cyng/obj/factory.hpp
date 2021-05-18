/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_FACTORY_HPP
#define CYNG_OBJ_FACTORY_HPP

#include <cyng/obj/core/wrapper.hpp>
#include <cyng/obj/object.h>
#ifdef _DEBUG_OBJECT
#include <cyng/io/ostream.h>
#endif

namespace cyng {

	/**
	 * produce the null object
	 */
	[[nodiscard]]
	object make_object(void);

	/**
	 * produce the EOD object
	 */
	[[nodiscard]]
	object make_eod();

	//
	//	forward declaration(s)
	//
	template < typename T >
	vector_t make_vector(std::vector< T > const& data);

	namespace policy {

		/**
		 * Generic
		 */
		template <typename T, typename... Args>
		struct boxing
		{
			using value_t = typename std::decay< T >::type;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;
			
			static ptr_t create(Args&&... args) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(std::forward<Args>(args)...), object::tracker_);
			}
		}; 

		/**
		 * Handling tuple types
		 */
		template <typename T, typename... Args>
		struct boxing<T, std::tuple< Args... >>
		{
			using tpl_type = std::tuple< Args... >;

			using value_t = typename std::decay< T >::type;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(Args&&... args) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(std::forward<Args>(args)...), object::tracker_);
			}
		};

		template <typename T>
		struct boxing< T >
		{
			using value_t = typename std::decay< T >::type;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(T&& v) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(std::forward<T>(v)), object::tracker_);
			}
		};
		template <typename T>
		struct boxing< T* >
		{
			using value_t = typename std::decay< T >::type;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(T const* v) {

				//
				//	take a copy and delete original value to prevent a memeory leak
				//
				ptr_t ptr = std::unique_ptr<wrapper_t, tracker>(new wrapper_t(*v), object::tracker_);
				delete v;
				return ptr;
			}
		};

		/**
		 * Handle C-style strings as std::string.
		 */
		template <>
		struct boxing<char const*>
		{
			using value_t = std::string;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			//	full specialized
			static ptr_t create(char const* p);
		};

		template <>
		struct boxing<char const* const&>
		{
			using value_t = std::string;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			//	full specialized
			static ptr_t create(char const* const& p);
		};

		/**
		 * Handle C-style strings as std::string.
		 */
		template <std::size_t N>
		struct boxing<const char(&)[N]>
		{
			using value_t = std::string;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(char const(&p)[N]) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(value_t(p, N - 1)), object::tracker_);
			}
		};

		template <typename T, typename A>
		struct boxing<std::vector<T, A>>
		{
			using value_t = vector_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(std::vector<T> const& vec) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(make_vector<T>(vec)), object::tracker_);
			}
		};

		template <typename T, typename A>
		struct boxing<const std::vector<T, A>>
		{
			using value_t = vector_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(std::vector<T> const& vec) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(make_vector<T>(vec)), object::tracker_);
			}
		};

		template <typename T, typename A>
		struct boxing<std::vector<T, A>&>
		{
			using value_t = vector_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(std::vector<T>& vec) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(make_vector<T>(vec)), object::tracker_);
			}
		};

		template <typename T, typename A>
		struct boxing<std::vector<T, A> const&>
		{
			using value_t = vector_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(std::vector<T> const& vec) {
				return std::unique_ptr<wrapper_t, tracker>(new wrapper_t(make_vector<T>(vec)), object::tracker_);
			}
		};

		//
		//	special case buffers
		//
		template <>
		struct boxing<buffer_t>
		{
			using value_t = buffer_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(buffer_t val);

			/**
			 * create a buffer from a string
			 */
			static ptr_t create(std::string const& val);
		};
		template <>
		struct boxing<buffer_t&>
		{
			using value_t = buffer_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(buffer_t& val);
		};
		template <>
		struct boxing<buffer_t const&>
		{
			using value_t = buffer_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(buffer_t const& val);
		};
		template <>
		struct boxing<buffer_t&&>
		{
			using value_t = buffer_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(buffer_t&& val);
		};

		//
		//	special case obis paths
		//
		template <>
		struct boxing<obis_path_t>
		{
			using value_t = obis_path_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(obis_path_t val);
		};
		template <>
		struct boxing<obis_path_t&>
		{
			using value_t = obis_path_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(obis_path_t& val);
		};
		template <>
		struct boxing<obis_path_t const&>
		{
			using value_t = obis_path_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(obis_path_t const& val);
		};
		template <>
		struct boxing<obis_path_t&&>
		{
			using value_t = obis_path_t;
			using wrapper_t = wrapper<value_t>;
			using ptr_t = std::unique_ptr<wrapper_t, tracker>;

			static ptr_t create(obis_path_t&& val);
		};

		/**
		 * Implementing the "don't wrap objects" rule
		 */
		template <>
		struct boxing<object>
		{
			static object create(object const& obj);
			static object create(object&& obj);
		};
		template <>
		struct boxing<object&>
		{
			static object create(object& obj);
		};
		template <>
		struct boxing<object const&>
		{
			static object create(object const& obj);
		};

	}


	/**
	 * Generic factory method.
	 * The compiler picks this function if specified target type T
	 * and source type(s) Args... are different.
	 */
	template <typename T, typename... Args>
	[[nodiscard]]
	object make_object(Args&&... args)
	{
		using policy_t = policy::boxing<T>;
		return object(policy_t::create(std::forward<Args>(args)...));
	}

	/**
	 * Same target and source type.
	 *
	 * @code
	 * factory<T>::create_object(T&&);
	 * @endcode
	 */
	template <typename T>
	[[nodiscard]]
	object make_object(T&& v)
	{
		using policy_t = policy::boxing<T>;
		return object(policy_t::create(std::forward<T>(v)));
	}

	/**
	 * No nested objects
	 */
	template <>
	[[nodiscard]]
	object make_object(object&& obj);

	/**
	 * No nested objects
	 * @return a copy of obj
	 */
	template <>
	[[nodiscard]]
	object make_object<object>(object const& obj);

	template <>
	[[nodiscard]]
	object make_object<object>(object& obj);

	/**
	 * Transform a C++ vector into an vector of objects.
	 * Precondition is existing factory method for data type T.
	 * 
	 * @see util.hpp
	 */
	template < typename T >
	[[nodiscard]]
	vector_t make_vector(std::vector< T > const& data)
	{
		vector_t vec;
		vec.reserve(data.size());
		std::transform(data.begin(), data.end(), std::back_inserter(vec), [](T val) {
			return make_object<T>(std::forward<T>(val));
			});
		return vec;
	}

	/**
	 * Identity cast
	 */
	[[nodiscard]]
	vector_t make_vector(vector_t&& vec);

	/**
	 * create an object of type boost::asio::ip::address
	 */
	[[nodiscard]]
	object address_factory(std::string);

}

#endif //	CYNG_OBJ_FACTORY_HPP
