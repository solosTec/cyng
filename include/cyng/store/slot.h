/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_SLOT_H
#define CYNG_STORE_SLOT_H

#include <cyng/task/channel.h>
#include <cyng/store/key.hpp>

#include <boost/uuid/uuid.hpp>

namespace cyng {

	class table;

	class slot;
	bool operator==(slot const&, slot const&);
	class slot
	{
		friend bool operator==(slot const&, slot const&);

	public:
		explicit slot(channel&);
		explicit slot(channel_ptr);
		explicit slot(channel_weak);
		slot(slot const&) = default;

		bool forward(table const*
			, key_t const&
			, data_t const&
			, std::uint64_t
			, boost::uuids::uuid);

		bool forward(table const* tbl
			, key_t const& key
			, attr_t const& attr
			, std::uint64_t gen
			, boost::uuids::uuid tag);

		bool forward(table const* tbl
			, key_t const& key
			, boost::uuids::uuid tag);

		bool forward(table const*
			, boost::uuids::uuid);
	private:
		channel_weak channel_;
	};

}
#endif

