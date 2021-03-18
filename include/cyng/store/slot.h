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

	//
	//	forward declaration(s)
	//
	//class slot;
	//bool operator==(slot const&, slot const&);

	class slot : public slot_interface
	{
		//friend bool operator==(slot const&, slot const&);

	public:
		explicit slot(channel&);
		explicit slot(channel_ptr);
		explicit slot(channel_weak);
		slot(slot const&) = default;

		virtual bool forward(table const*
			, key_t const&
			, data_t const&
			, std::uint64_t
			, boost::uuids::uuid) override;

		virtual bool forward(table const* tbl
			, key_t const& key
			, attr_t const& attr
			, std::uint64_t gen
			, boost::uuids::uuid tag) override;

		virtual bool forward(table const* tbl
			, key_t const& key
			, boost::uuids::uuid tag) override;

		virtual bool forward(table const*
			, boost::uuids::uuid) override;

	private:
		channel_weak channel_;
	};

	slot_ptr make_slot(channel&);
	slot_ptr make_slot(channel_ptr);
	slot_ptr make_slot(channel_weak);

}
#endif

