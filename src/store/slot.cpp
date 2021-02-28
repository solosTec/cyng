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
		if (sp)	sp->dispatch("insert", cyng::make_tuple(
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

		if (sp)	sp->dispatch("modify", cyng::make_tuple(
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
		return sp.operator bool();

	}

	bool slot::forward(table const* tbl
		, boost::uuids::uuid tag) {

		auto sp = channel_.lock();
		return sp.operator bool();
	}

	bool operator==(slot const& s1, slot const& s2) {
		auto l1 = s1.channel_.lock();
		auto l2 = s2.channel_.lock();
		return (l1 && l2)
			? l1.get() == l2.get()
			: false
			;
	}

}
