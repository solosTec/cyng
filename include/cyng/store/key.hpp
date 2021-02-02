/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef DOCC_STORE_KEY_HPP
#define DOCC_STORE_KEY_HPP

#include <cyng/obj/object.h>
#include <cyng/obj/intrinsics/container.h>
#include <cyng/obj/util.hpp>
#include <cyng/obj/container_cast.hpp>

#include <shared_mutex>


namespace docscript {

	using key_t = vector_t;
	using key_list_t = std::list<key_t>;	//	not unique
	using data_t = key_t;

	template < typename ...Args >
	key_t key_generator(Args&&... args)
	{
		return container_transform<vector_t>(docscript::make_tuple(std::forward<Args>(args)...));
	}

	template < typename ...Args >
	data_t data_generator(Args&&... args)
	{
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

//std::equal_to<key_t>
#include <functional>

namespace std {

	template <>
	class equal_to<docscript::key_t> {
	public:
		bool operator()(docscript::key_t const& pk1, docscript::key_t const& pk2) const noexcept;
	};
}

#endif

