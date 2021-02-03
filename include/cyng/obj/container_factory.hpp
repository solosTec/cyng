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
		return make_object(make_tuple(std::forward<Args>(args)...));
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

}

#endif //	CYNG_OBJ_FACTORY_HPP
