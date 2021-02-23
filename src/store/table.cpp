#include <cyng/store/table.h>


namespace cyng {

	table::table(meta_store const& m)
		: pub()
		, meta_(m)
		, data_()
		, m_()
	{}

	table::~table()
	{}

	std::size_t table::size() const noexcept {
		return data_.size();
	}

	void table::reserve(std::size_t count) {
		data_.reserve(count);
	}

	meta_store const& table::meta() const noexcept {
		return meta_;
	}

	bool table::is_auto() const {
		return false;
	}

	void table::clear(boost::uuids::uuid source)	{
		data_.clear();
		//index_.clear();

		//
		//	ToDo: publish
		//

		//this->pub::clear_signal_(this, source);
	}

	bool table::insert(key_t const& key
		, data_t&& data
		, std::uint64_t generation
		, boost::uuids::uuid source) {

		if (meta().check_integrity(key, data)) {
			if (emplace(key, std::move(data), generation, false)) {

				//
				//	ToDo: publish
				//
				return true;
			}
		}
		return false;
	}

	bool table::emplace(key_t const& key
		, data_t&& data
		, std::uint64_t generation
		, bool merge) {

		auto const r = data_.emplace(std::piecewise_construct
			, std::forward_as_tuple(key)
			, std::forward_as_tuple(data, generation));

		if (!r.second && merge) {
			//
			//	update
			//
			update(r.first, key, std::move(data));
			return true;
		}
		//if (r.second && meta().has_sk()) {

		//	//
		//	//	ToDo: update secondary key(s)
		//	//
		//}
		return r.second;
	}

	bool table::merge(key_t const& key
		, data_t&& data
		, std::uint64_t generation
		, boost::uuids::uuid source) {

		if (meta().check_integrity(key, data)) {
			if (emplace(key, std::move(data), generation, true)) {

				//
				//	ToDo: publish
				//	ToDo: update secondary key(s)
				//
				return true;
			}
		}
		return false;
	}

	void table::update(table_t::iterator pos
		, key_t const& key
		, data_t&& data)
	{
		//
		//	write lock this record
		//
		std::unique_lock<std::shared_mutex> ulock(pos->second.m_);

		//
		//	update generation
		//
		++pos->second.generation_;

		//
		//	update data
		//
		pos->second.data_ = std::move(data);

	}

	bool table::exist(key_t const& key) const {
		return data_.find(key) != data_.end();
	}

	record table::lookup(key_t const& key) const {
		auto const pos = data_.find(key);
		if (pos != data_.end()) {
			return { meta(), key, pos->second.data_, pos->second.generation_ };
		}
		return { meta() };
	}

	record table::lookup(key_t const& key, std::size_t idx) const {

		//
		//	ToDo: check index of secondary
		//	ToDo: check key size
		//	ToDo: search for key and retrun result
		//
		return { meta() };
	}

	bool table::erase(key_t const& key, boost::uuids::uuid source) {

		//
		//	update secondary key
		//

		if (data_.erase(key) != 0)	{


			//
			//	ToDo: publish
			//
			return true;
		}
		return false;

	}

	std::size_t table::loop(std::function<bool(record&&, std::size_t)> f) const {

		std::size_t counter{ 0 };
		for (auto const& row : data_)
		{
			//
			//	read lock this record
			//
			std::shared_lock<std::shared_mutex> ulock(row.second.m_);

			if (!f(record(meta(), row.first, row.second.data_, row.second.generation_), counter))	break;
			++counter;
		}
		return counter;
	}
}
