#include <cyng/store/pub.h>


namespace cyng {

	pub::pub()
		: slots_insert_()
		, slots_modify_()
		, slots_remove_()
		, slots_clear_()
	{}

	void pub::connect(slot_ptr sp) {

		connect_only(sp);

		//
		//	load initial data set
		//
		charge(sp);
	}

	void pub::connect_only(slot_ptr sp) {

		BOOST_ASSERT(sp);

		connect_insert(sp);
		connect_modify(sp);
		connect_remove(sp);
		connect_clear(sp);
	}


	void pub::connect_insert(slot_ptr sp) {
		slots_insert_.push_back(sp);
	}

	void pub::connect_modify(slot_ptr sp) {
		slots_modify_.push_back(sp);
	}

	void pub::connect_remove(slot_ptr sp) {
		slots_remove_.push_back(sp);
	}

	void pub::connect_clear(slot_ptr sp) {
		slots_clear_.push_back(sp);
	}

	void pub::disconnect(slot_ptr sp) {
		cyng::disconnect(slots_insert_, sp);
		cyng::disconnect(slots_modify_, sp);
		cyng::disconnect(slots_remove_, sp);
		cyng::disconnect(slots_clear_, sp);
	}

	void pub::forward(table const* tbl
		, key_t const& key
		, data_t const& data
		, std::uint64_t gen
		, boost::uuids::uuid tag) {

		std::forward_list<slot_ptr> expired;

		for (auto& sp : slots_insert_) {
			if (!sp->forward(tbl, key, data, gen, tag)) {
				expired.push_front(sp);
			}
		}

		tidy(slots_insert_, expired);
	}

	void pub::forward(table const* tbl
		, key_t const& key
		, attr_t const& attr
		, std::uint64_t gen
		, boost::uuids::uuid tag) {

		std::forward_list<slot_ptr> expired;

		for (auto& sp : slots_insert_) {
			if (!sp->forward(tbl, key, attr, gen, tag)) {
				expired.push_front(sp);
			}
		}

		tidy(slots_insert_, expired);

	}

	/**
	 * signal an remove event
	 */
	void pub::forward(table const* tbl
		, key_t const& key
		, boost::uuids::uuid tag) {

		std::forward_list<slot_ptr> expired;

		for (auto& sp : slots_insert_) {
			if (!sp->forward(tbl, key, tag)) {
				expired.push_front(sp);
			}
		}

		tidy(slots_insert_, expired);

	}

	/**
	 * signal an clear event
	 */
	void pub::forward(table const* tbl
		, boost::uuids::uuid tag) {

		std::forward_list<slot_ptr> expired;

		for (auto& sp : slots_insert_) {
			if (!sp->forward(tbl, tag)) {
				expired.push_front(sp);
			}
		}

		tidy(slots_insert_, expired);

	}

	bool disconnect(std::vector<slot_ptr>& vec, slot_ptr s) {
		auto pos = std::find(std::begin(vec), std::end(vec), s);
		if (pos != std::end(vec)) {
			vec.erase(pos);
			return true;
		}
		return false;
	}

	void tidy(std::vector<slot_ptr>& vec, std::forward_list<slot_ptr>& expired) {

		for (auto& s : expired) {
			disconnect(vec, s);
		}
	}
}
