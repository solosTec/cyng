#include <cyng/store/meta.h>
#include <boost/algorithm/string.hpp>

namespace cyng {

	column::column(std::string const& name, std::uint16_t type)
		: name_(name)
		, type_(type)
	{}

	column_sql::column_sql(column const& col, std::size_t width)
		: column(col)
		, width_(width)
	{}

	column_sql::column_sql(std::string const& name, std::uint16_t type, std::size_t width)
		: column(name, type)
		, width_(width)
	{}

	//	mem => sql
	//	add a column "gen" after the key
	meta_sql to_sql(meta_store const& m, std::vector<std::size_t> const& width) {
		BOOST_ASSERT(m.size() == width.size());

		std::vector < column_sql >	vec;
		vec.reserve(m.size() + 1);
		bool gen = false;
		m.loop([&](std::size_t idx, column const& col, bool pk)->void {
			if (!pk && !gen) {
				//	insert gen
				gen = true;
				vec.push_back(column_sql("gen", TC_UINT64, 0u));
			}

			if (width.size() > vec.size()) {
				auto const w = width.at(vec.size());
				vec.push_back(column_sql(col, w));
			}
			else {
				vec.push_back(column_sql(col, 0));
			}
		});
		return meta_sql(m.get_name(), std::begin(vec), std::end(vec), m.key_size());
	}

	//	sql => mem
	//	remove column "gen" after the key
	meta_store to_mem(meta_sql const& m) {
		std::vector < column >	vec;
		vec.reserve(m.size());
		m.loop([&vec](std::size_t idx, column_sql const& col, bool pk)->void {

			//	skip column gen
			if (!boost::algorithm::equals(col.name_, "gen")) {
				vec.push_back(column(col.name_, col.type_));
			}
		});
		return meta_store(m.get_name(), std::begin(vec), std::end(vec), m.key_size());

	}

}
