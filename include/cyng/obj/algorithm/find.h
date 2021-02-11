/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_FIND_H
#define CYNG_OBJ_ALGORITHM_FIND_H

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/value_cast.hpp>

#include <type_traits>

namespace cyng {

	/**
	 * Index based access for attributes.
	 *
	 * @return the first object in the container that is an attribute with
	 * the specified index. If the indexed object
	 * is not an attribute or out of range, the returned object is null.
	 *
	 * If the object is not an attribute an index based access (subscription operator)
	 * will be tried. But the attribute index takes precedence.
	 *
	 * Example:
	 *
	 * @code
	 * 	cyng::vector_t v;
	 * 	v.push_back(cyng::set_factory(3, "three"));
	 * 	v.push_back(cyng::set_factory(1, "one"));
	 * 	v.push_back(cyng::set_factory(2, "two"));
	 * 	v.push_back(cyng::set_factory(42, "forty-two"));
	 * 	v.push_back(cyng::set_factory(8, "eight"));
	 *
	 * 	cyng::object obj = cyng::find(v, 42);
	 *  BOOST_CHECK(cyng::is_of_type<cyng::types::CYNG_STRING)>(obj));
	 * 	auto ptr = cyng::object_cast<std::string>(obj);
	 * 	BOOST_CHECK(ptr != nullptr);
	 * 	BOOST_CHECK_EQUAL(*ptr, "forty-two");
	 *
	 * @endcode
	 */
	object find(vector_t const&, std::size_t);

	/**
	 * @see Look for an element of type attribute. If no such
	 * attribute exists, take the Nth element as specified.
	 */
	object find(tuple_t const&, std::size_t);

	/**
	 * @see Look for an element of type attribute. If no such
	 * attribute exists, take the Nth element as specified.
	 */
	object find(deque_t const&, std::size_t);

	/**
	 * @return attribute with the specified index.
	 */
	object find(attr_map_t const&, std::size_t);

	/**
	 * @return the value of the attribute if the index is matching
	 * the attribute index. Otherwise a null-objects will return.
	 */
	object find(attr_t const&, std::size_t);

	/**
	 * Detect the type of the object and call the appropriate
	 * find() method.
	 */
	object find(object const&, std::size_t);

	/**
	 * Vectors are searched for parameters with this name and for simple vectors
	 * of strings. In case of parameters the the function returns the parameter
	 * value (if one was found). A vector of strings return the found string.
	 *
	 * If nothing has matched the function returns a null object.
	 *
	 * It seems that is doesn't make much sense to return the string that we were looking for.
	 * But to return a boolean value could be confusing too. So we return the value that
	 * was found. To indicate an successful search the safe bool conversion of object could
	 * be usefull
	 */
	object find(vector_t const&, std::string const&);

	/**
	 * Search the deque for a parameter with the specified name.
	 * If nothing was found it returns the null object.
	 */
	object find(deque_t const&, std::string const&);

	/**
	 * Name based access for parameters.
	 *
	 * @return the first object in the container that is a parameter with
	 * the specified name.
	 */
	object find(tuple_t const&, std::string const&);

	/**
	 * @return the value of the parameter with the specified name
	 * of the null object.
	 */
	object find(param_map_t const&, std::string const&);

	/**
	 * @return the value of the Parameter if the name is matching
	 * the parameter name. Otherwise a null-objects will return.
	 */
	object find(param_t const&, std::string const&);

	/**
	 * Detect the type of the object and call the appropriate
	 * find() method.
	 */
	object find(object const&, std::string const&);

	/**
	 * Convinience function
	 * Key must be of type std::string or an unsigned integer
	 */
	template <typename T, typename C, typename K>
	T find_value(C const& c, K key, T const& def)
	{
		static_assert(std::is_unsigned_v<K>
			|| std::is_same_v<K, std::string>, "Key must be of type std::string or an unsigned integer");

		return value_cast<T>(find(c, key), def);
	}

}

#endif //	CYNG_OBJ_ALGORITHM_FIND_H
