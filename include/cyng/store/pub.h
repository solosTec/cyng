/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_PUB_H
#define CYNG_STORE_PUB_H

#include <cyng/store/key.hpp>

#include <functional>
#include <boost/uuid/uuid.hpp>

namespace cyng {

	class table;
	class pub
	{
	public:
		using insert_signal = std::function<void(table const*
			, key_t const&
			, data_t const&
			, std::uint64_t
			, boost::uuids::uuid)>;
		using remove_signal = std::function<void(table const*
			, key_t const&
			, boost::uuids::uuid)>;
		using clear_signal = std::function<void(table const*, boost::uuids::uuid)>;
		using modify_signal = std::function<void(table const*
			, key_t const&
			, attr_t const&
			, std::uint64_t
			, boost::uuids::uuid)>;

	public:
		pub();
	};
}
#endif

