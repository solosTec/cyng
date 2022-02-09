#include <cyng/store/table.h>

#include  <mutex>

#include <boost/uuid/nil_generator.hpp>

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
		//	publish
		//
		this->pub::forward(this, source);

	}

	bool table::insert(key_t const& key
		, data_t const& data
		, std::uint64_t generation
		, boost::uuids::uuid source) {

		if (meta().check_integrity(key, data)) {
			if (emplace(key, data, generation)) {

				//
				//	publish
				//
				this->pub::forward(this, key, data, generation, source);
				return true;
			}
		}
		return false;
	}

	bool table::emplace(key_t const& key
		, data_t const& data
		, std::uint64_t generation) {

		//
		//	r.second is true an insertion happend 
		//	otherwise an update is required
		//
		auto const r = data_.emplace(std::piecewise_construct
			, std::forward_as_tuple(key)
			, std::forward_as_tuple(data, generation));

		//
		//	ToDo: update secondary key(s)
		//
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

			auto const r = data_.emplace(std::piecewise_construct
				, std::forward_as_tuple(key)
				, std::forward_as_tuple(data, generation));

			if (r.second) {

				//
				//	publish insert
				//
				this->pub::forward(this
					, key
					, data
					, generation
					, source);


				//	ToDo: update secondary key(s)
				//
				return true;
			}
			else {

				//
				//	update record and publish modifications
				//
				update(r.first
					, key
					, std::move(data)
					, source);
			}
		}
		return false;
	}

	void table::update(table_t::iterator pos
		, key_t const& key
		, data_t&& data
		, boost::uuids::uuid source)
	{
		//
		//	write lock this record
		//
		std::unique_lock<std::shared_mutex> ulock(pos->second.m_);

		//
		//	update data
		//
		BOOST_ASSERT(data.size() == pos->second.data_.size());
		for (std::size_t idx = 0; idx < data.size(); ++idx) {

			if (pos->second.data_.at(idx) != data.at(idx)) {

				//
				//	publish
				//
				attr_t attr(idx, data.at(idx));
				this->pub::forward(this, key, attr, pos->second.generation_, source);

				//
				//	apply
				//
				swap(pos->second.data_.at(idx), data.at(idx));

				//
				//	update generation
				//
				++pos->second.generation_;

			}
		}
	}

	void table::charge(slot_ptr sp) {

		//
		//	start transaction
		//
		sp->forward(this, true);

		//
		//	table already locked
		//

		auto const source = boost::uuids::nil_uuid();
		for (auto const& row : data_)
		{
			//
			//	read lock this record
			//
			std::shared_lock<std::shared_mutex> ulock(row.second.m_);

			sp->forward(this
				, row.first
				, row.second.data_
				, row.second.generation_
				, source);
		}

		//
		//	commit transaction
		//
		sp->forward(this, false);

	}

	bool table::modify(key_t const& key
		, attr_t&& attr
		, boost::uuids::uuid source) {

		auto pos = data_.find(key);
		if (pos != data_.end()) {

			//
			//	write lock this record
			//
			std::unique_lock<std::shared_mutex> ulock(pos->second.m_);

			if (pos->second.data_.at(attr.first) != attr.second) {

				//
				//	publish
				//
				//attr_t attr(idx, data.at(idx));
				this->pub::forward(this, key, attr, pos->second.generation_, source);

				//
				//	apply
				//
				swap(pos->second.data_.at(attr.first), attr.second);

				//
				//	update generation
				//
				++pos->second.generation_;

				return true;

			}
		}
		return false;
	}

	bool table::modify(key_t const& key
		, attr_map_t&& am
		, boost::uuids::uuid source) {

		auto pos = data_.find(key);
		if (pos != data_.end()) {

			//
			//	write lock this record
			//
			std::unique_lock<std::shared_mutex> ulock(pos->second.m_);

			for (auto attr : am) {
		
				BOOST_ASSERT(attr.first != std::numeric_limits<std::size_t>::max());
				
				if ((attr.first < meta().body_size())
					&& (pos->second.data_.at(attr.first) != attr.second)) {

					//
					//	publish
					//
					this->pub::forward(this, key, attr, pos->second.generation_, source);

					//
					//	apply
					//
					swap(pos->second.data_.at(attr.first), attr.second);

					//
					//	update generation
					//
					++pos->second.generation_;

				}
			}
			return true;
		}
		return false;
	}


	bool table::modify(key_t const& key
		, param_t const& param
		, boost::uuids::uuid source) {

		auto const idx = meta().get_body_index_by_name(param.first);
		return modify(key, attr_t(idx, param.second), source);
	}

	bool table::modify(key_t const& key
		, param_map_t const& pm
		, boost::uuids::uuid source) {

		attr_map_t am;
		std::transform(pm.begin(), pm.end(), std::inserter(am, am.end()), [this](param_map_t::value_type const& param) {
			auto const idx = meta().get_body_index_by_name(param.first);
			return attr_t(idx, param.second);
			});

		return modify(key, std::move(am), source);
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

		auto const pos = data_.find(key);
		if (pos != data_.end())	{

			//
			//	publish
			//
			forward(this, key, pos->second.data_, source);

			//
			//	remove
			//
			data_.erase(pos);
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

	vector_t table::to_vector(bool col_names) const
	{
		vector_t vec;

		if (col_names) {
			vec.reserve(size() + 1);

			//
			//	generate header entry
			//
			tuple_t header;
			meta_.loop([&header](std::size_t, cyng::column const& col, bool) {

				header.push_back(make_object(col.name_));

				});

			//
			//	insert header record
			//
			vec.push_back(make_object(header));
		}
		else {
			vec.reserve(size());
		}

		//
		//	append all records
		//
		loop([&](cyng::record&& rec, std::size_t)->bool {

			auto const key = rec.key();
			auto const data = rec.data();
			
			tuple_t tpl(key.begin(), key.end());
			tpl.insert(tpl.end(), data.begin(), data.end());
			vec.push_back(make_object(tpl));

			return true;	//	continue
			});

		return vec;
	}

}
