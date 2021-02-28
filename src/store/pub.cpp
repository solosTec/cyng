#include <cyng/store/pub.h>


namespace cyng {

	pub::pub()
		: slots_insert_()
		, slots_modify_()
		, slots_remove_()
		, slots_clear_()
	{}

	void pub::connect(slot s) {
		connect_insert(s);
		connect_modify(s);
		connect_remove(s);
		connect_clear(s);
	}

	void pub::connect_insert(slot s) {
		slots_insert_.push_back(s);
	}

	void pub::connect_modify(slot s) {
		slots_modify_.push_back(s);
	}

	void pub::connect_remove(slot s) {
		slots_remove_.push_back(s);
	}

	void pub::connect_clear(slot s) {
		slots_clear_.push_back(s);
	}

	void pub::disconnect(slot s) {
		cyng::disconnect(slots_insert_, s);
		cyng::disconnect(slots_modify_, s);
		cyng::disconnect(slots_remove_, s);
		cyng::disconnect(slots_clear_, s);
	}

	void pub::forward(table const* tbl
		, key_t const& key
		, data_t const& data
		, std::uint64_t gen
		, boost::uuids::uuid tag) {

		std::forward_list<slot> expired;

		for (auto& s : slots_insert_) {
			if (!s.forward(tbl, key, data, gen, tag)) {
				expired.push_front(s);
			}
		}

		tidy(slots_insert_, expired);
	}

	void pub::forward(table const* tbl
		, key_t const& key
		, attr_t const& attr
		, std::uint64_t gen
		, boost::uuids::uuid tag) {

		std::forward_list<slot> expired;

		for (auto& s : slots_insert_) {
			if (!s.forward(tbl, key, attr, gen, tag)) {
				expired.push_front(s);
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

		std::forward_list<slot> expired;

		for (auto& s : slots_insert_) {
			if (!s.forward(tbl, key, tag)) {
				expired.push_front(s);
			}
		}

		tidy(slots_insert_, expired);

	}

	/**
	 * signal an clear event
	 */
	void pub::forward(table const* tbl
		, boost::uuids::uuid tag) {

		std::forward_list<slot> expired;

		for (auto& s : slots_insert_) {
			if (!s.forward(tbl, tag)) {
				expired.push_front(s);
			}
		}

		tidy(slots_insert_, expired);

	}

	bool disconnect(std::vector<slot>& vec, slot s) {
		auto pos = std::find(std::begin(vec), std::end(vec), s);
		if (pos != std::end(vec)) {
			vec.erase(pos);
			return true;
		}
		return false;
	}

	void tidy(std::vector<slot>& vec, std::forward_list<slot>& expired) {

		for (auto& s : expired) {
			disconnect(vec, s);
		}
	}
}
