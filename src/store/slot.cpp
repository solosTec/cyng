#include <cyng/store/slot.h>
#include <cyng/store/table.h>

namespace cyng {

	slot::slot(channel& ch)
		: channel_(ch.weak_from_this())
	{}

	slot::slot(channel_ptr cp)
		: channel_(cp.get()->weak_from_this())
	{}

	slot::slot(channel_weak ch)
		: channel_(ch)
	{}

	bool slot::forward(table const* tbl
		, key_t const& key
		, data_t const& data
		, std::uint64_t gen
		, boost::uuids::uuid tag) {

		auto sp = channel_.lock();
		if (sp)	sp->dispatch("db.insert", cyng::make_tuple(
			tbl,
//			tbl->meta().get_name(), 
			key,
			data,
			gen,
			tag
		));

		return sp.operator bool();
	}

	bool slot::forward(table const* tbl
		, key_t const& key
		, attr_t const& attr
		, std::uint64_t gen
		, boost::uuids::uuid tag) {

		auto sp = channel_.lock();

		if (sp)	sp->dispatch("db.modify", cyng::make_tuple(
			tbl,
			//			tbl->meta().get_name(), 
			key,
			attr,
			gen,
			tag
		));

		return sp.operator bool();
	}

	bool slot::forward(table const* tbl
		, key_t const& key
		, boost::uuids::uuid tag) {

		auto sp = channel_.lock();

		if (sp)	sp->dispatch("db.remove", cyng::make_tuple(
			tbl,
			key,
			tag
		));

		return sp.operator bool();

	}

	bool slot::forward(table const* tbl
		, boost::uuids::uuid tag) {

		auto sp = channel_.lock();
		if (sp)	sp->dispatch("db.clear", cyng::make_tuple(
			tbl,
			tag
		));
		return sp.operator bool();
	}

	bool slot::forward(table const* tbl
		, bool trx) {

		auto sp = channel_.lock();
		if (sp)	sp->dispatch("db.trx", cyng::make_tuple(
			tbl,
			trx
		));
		return sp.operator bool();
	}


	slot_ptr make_slot(channel& ch) {
		return std::make_shared<slot>(ch);
	}
	slot_ptr make_slot(channel_ptr cp) {
		return std::make_shared<slot>(cp);
	}
	slot_ptr make_slot(channel_weak cw) {
		auto cp = cw.lock();
		return (cp)
			? make_slot(cp)
			: slot_ptr()
			;
	}

}
