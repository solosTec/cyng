/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_OBJ_ALGORITHM_READER_HPP
#define CYNG_OBJ_ALGORITHM_READER_HPP

#include <cyng/obj/object.h>
#include <cyng/obj/algorithm/find.h>
#include <cyng/obj/value_cast.hpp>

#include <type_traits>

namespace cyng {

	template <typename T>
	class reader
	{
	public:
		reader(T&& container) noexcept
			: container_(std::forward<T>(container))	//	move
			, owner_(std::is_rvalue_reference<decltype(container)>::value)
		{}
		reader(reader const&) = default;

		/**
		 * Simple element access by name.
		 * Works for paramater maps and searchs in tuples, vectors
		 * and sets for an parameter with the specified name.
		 */
		object get(std::string const& name) const	{
			return find(container_, name);
		}

		template< typename U>
		auto get(std::string const& name, U&& def) const -> decltype(value_cast<U>(get(name), def)) {
			return value_cast<U>(get(name), def);
		}

		/**
		 * Simple element access by index
		 * Works for attribute maps and vectors. Searchs in tuples, vectors
		 * and sets for an attribute with the specified index.
		 * Note: The search for attributes has a higher precedence in vectors
		 * thean access by index.
		 */
		object get(std::size_t idx) const {
			return find(container_, idx);
		}

		template< typename U>
		auto get(std::size_t idx, U&& def) const -> decltype(value_cast<U>(get(idx), def)) {
			return value_cast<U>(get(idx), def);
		}

		/**
		 * @return size of the container
		 */
		std::size_t size() const noexcept {
			return container_.size();
		}

		/**
		 * array subscript operator. Allows continuation.
		 */
		reader<object> operator[](std::string const& name) const {
			return get(name);	//	implicit cast
		}

		/**
		 * array subscript operator. Allows continuation.
		 */
		reader<object> operator[](std::size_t idx) const {
			return get(idx);	//	implicit cast
		}

		/**
		 * When using continuation the container is often a simple object.
		 */
		T get() const {
			return container_;
		}

		bool is_owner() const {
			return owner_;
		}

	private:
		T container_;
		bool const owner_;
	};

	template <typename T>
	reader<T> make_reader(T&& c) noexcept {
		return reader<T>(std::forward<T>(c));
	}

}

#endif 
