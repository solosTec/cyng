#include <cyng/store/auto_table.h>


namespace cyng {

	auto_table::auto_table(meta_store const& m, key_t const& key, auto_key_f f)
		: table(m)
		, current_key_(key)
		, next_key_(f)
	{}

	auto_table::~auto_table()
	{}

	std::pair< key_t, bool > auto_table::insert(data_t&& data
		, boost::uuids::uuid source) {

		//
		//	calculate next key
		//
		current_key_ = next_key_(this, current_key_);

		return { current_key_, table::insert(current_key_
			, std::move(data)
			, 1u
			, source) };
	}

	bool auto_table::is_auto() const {
		return true;
	}

}
