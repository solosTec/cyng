/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_KEY_HPP
#define CYNG_STORE_KEY_HPP

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/util.hpp>
#include <cyng/obj/container_cast.hpp>

#include <shared_mutex>
#include <tuple>


namespace cyng {

	using key_t = vector_t;
	using key_list_t = std::set<key_t>;	//	unique
	using data_t = key_t;

	template < typename ...Args >
	[[nodiscard]]
	key_t key_generator(Args&&... args) {
		return { make_object<Args>(std::forward<Args>(args))... };
	}

	template < typename ...Args >
	key_t extend_key(key_t key, Args&&... args) {
		constexpr auto size = sizeof... (Args);
		key.reserve(key.size() + size);
		auto k = key_generator(std::forward<Args>(args)...);
		std::move(k.begin(), k.end(), std::back_inserter(key));
		return key;
	}

	template < typename ...Args >
	data_t data_generator(Args&&... args) {
		return key_generator(std::forward<Args>(args)...);
	}

	//
	//	define a data type for the table body
	//
	struct body
	{
		body();
		body(data_t&&, std::uint64_t);
		body(data_t const&, std::uint64_t);
		body(body const&) = delete;

		data_t	data_;
		std::uint64_t generation_;
		mutable std::shared_mutex m_;

	};
}

#include <functional>

namespace std {

	template <>
	class equal_to<cyng::key_t> {
	public:
		bool operator()(cyng::key_t const& pk1, cyng::key_t const& pk2) const noexcept;
	};

	template <>
	class less<cyng::key_t> {
	public:
		bool operator()(cyng::key_t const& c1, cyng::key_t const& c2) const noexcept;
	};

}

#endif

