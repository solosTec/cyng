#include <cyng/store/record.h>
#include <cyng/obj/util.hpp>

namespace cyng {

	record::record(meta_store const& m)
		: meta_(m)
		, key_()
		, data_()
		, gen_(0)
	{}

	record::record(meta_store const& m, key_t const& key, data_t const& data, std::uint64_t gen)
		: meta_(m)
		, key_(key)	//	copy
		, data_(data)	//	copy
		, gen_(gen)
	{}

	bool record::empty() const {
		return meta_.check_integrity(key_, data_);
	}

	key_t const& record::key() const {
		return key_;
	}

	/**
	 * @return the data part of the record.
	 */
	data_t const& record::data() const {
		return data_;
	}

	/*
	 * @return generation counter
	 */
	std::uint64_t record::get_generation() const {
		return gen_;
	}

	tuple_t record::to_tuple() const {

		param_map_t key, data;
		meta_.loop([&](std::size_t idx, column const& col, bool pk) {
			if (pk) {
				key[col.name_] = key_.at(idx);
			}
			else {
				data[col.name_] = data_.at(idx);
			}
			});

		//
		//	generation_ is part of the data body
		//
		data["gen"] = make_object(gen_);

		return cyng::make_tuple(
			make_param("key", key),
			make_param("data", data)
		);
	}

	object record::at(std::string name) const {
		auto const idx = meta_.get_index_by_name(name);
		return at(idx);
	}

	object record::get_data(std::size_t idx) const {
		return (idx < data_.size())
			? data_.at(idx)
			: object()
			;
	}

	/**
	 * access by column index (pk included)
	 */
	object record::at(std::size_t idx) const {
		if (idx < meta_.size()) {
			return (meta_.is_pk(idx))
				? key_.at(idx)
				: get_data(idx - meta_.key_size())
				;
		}
		return object();
	}
}

