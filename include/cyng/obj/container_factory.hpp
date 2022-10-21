/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_CONTAINER_FACTORY_HPP
#define CYNG_OBJ_CONTAINER_FACTORY_HPP

#include <cyng/obj/factory.hpp>
#include <cyng/obj/util.hpp>

namespace cyng {

	/**
	 * Create an object from each parameter element and add
	 * it to a list of objects (tuple_t)
	 */
	template < typename ...Args >
	[[nodiscard]]
	object tuple_factory(Args&&... args) {
		return make_object(cyng::make_tuple(std::forward<Args>(args)...));
	}


	template < typename T >
	[[nodiscard]]
	object vector_factory(std::vector< T > const& data) {
		return make_object(make_vector(data));
	}

	template < typename ...Args >
	[[nodiscard]]
	object deque_factory(Args&&... args) {
		return make_object(make_deque(std::forward<Args>(args)...));
	}

	/**
	 * Create an object of type attribute
	 */
	template < typename... Args >
	[[nodiscard]]
	object pair_factory(std::size_t idx, Args&&... args)
	{
		return make_object(make_attr(idx, std::forward<Args>(args)...));
	}

	/**
	 * Create an object of type parameter
	 */
	template < typename... Args >
	[[nodiscard]]
	object pair_factory(std::string const& name, Args&&... args)
	{
		BOOST_ASSERT_MSG(!name.empty(), "parameter without a name");
		return make_object(make_param(name, std::forward<Args>(args)...));
	}

	/**
	 * example
	 * @code
	 attr_map_t m = attr_map_factory(1, "1")(2, "2");
	 * @endcode
	 */
	class attr_map_factory
	{
	public:
		attr_map_factory();

		template < typename T >
		attr_map_factory(std::size_t key, T&& v)
			: map_()
		{
			map_.emplace(key, make_object(v));
		}

		template < typename T >
		attr_map_factory operator()(std::size_t key, T&& v)
		{
			map_.emplace(key, make_object(v));
			return *this;
		}

		object operator()() const;
		operator attr_map_t() const;

	private:
		attr_map_t map_;
	};

	/**
	 * example
	 * @code
	 param_map_t m = param_map_factory("1", 1)("2", 2);
	 * @endcode
	 */
	class param_map_factory
	{
	public:
		param_map_factory();

		template < typename T >
		param_map_factory(std::string const& key, T&& v)
			: map_()
		{
			BOOST_ASSERT_MSG(!key.empty(), "parameter without a name");
			map_.emplace(key, make_object(v));
		}

		template < typename T >
		param_map_factory operator()(std::string const& key, T&& v)
		{
			BOOST_ASSERT_MSG(!key.empty(), "parameter without a name");
			map_.emplace(key, make_object(v));
			return *this;
		}

		object operator()() const;
		operator param_map_t() const;

	private:
		param_map_t map_;
	};

	/**
	 * example
	 * @code
	 prop_map_t m = prop_map_factory(make_obis(1,2,3,4,5,6), 1)(make_obis(6,5,4,3,2,1), 2);
	 * @endcode
	 */
	class prop_map_factory
	{
	public:
		prop_map_factory();

		template < typename T >
		prop_map_factory(obis const& key, T&& v)
			: map_()
		{
			BOOST_ASSERT_MSG(!is_nil(key), "property without a name");
			map_.emplace(key, make_object(v));
		}

		template < typename T >
		prop_map_factory operator()(obis const& key, T&& v)
		{
			BOOST_ASSERT_MSG(!is_nil(key), "property without a name");
			map_.emplace(key, make_object(v));
			return *this;
		}

		object operator()() const;
		operator prop_map_t() const;

	private:
		prop_map_t map_;
	};
}

#endif //	CYNG_OBJ_FACTORY_HPP
