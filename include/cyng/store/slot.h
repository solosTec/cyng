/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_SLOT_H
#define CYNG_STORE_SLOT_H

#include <cyng/store/slot_interface.h>
#include <cyng/task/channel.h>

namespace cyng {

	class slot : public slot_interface
	{

	public:
		explicit slot(channel&);
		explicit slot(channel_ptr);
		explicit slot(channel_weak);
		slot(slot const&) = default;

		/**
		 * insert event
		 */
		virtual bool forward(table const*
			, key_t const&
			, data_t const&
			, std::uint64_t
			, boost::uuids::uuid) override;

		/**
		 * modify event
		 */
		virtual bool forward(table const* tbl
			, key_t const& key
			, attr_t const& attr
			, data_t const&
			, std::uint64_t gen
			, boost::uuids::uuid tag) override;

		/**
		 * remove event
		 */
		virtual bool forward(table const* tbl
			, key_t const& key
			, data_t const& data
			, boost::uuids::uuid tag) override;

		virtual bool forward(table const*
			, boost::uuids::uuid) override;

		virtual bool forward(table const*
			, bool) override;


	private:
		channel_weak channel_;
	};

	slot_ptr make_slot(channel&);
	slot_ptr make_slot(channel_ptr);
	slot_ptr make_slot(channel_weak);

}
#endif

