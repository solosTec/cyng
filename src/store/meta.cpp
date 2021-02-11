#include <cyng/store/meta.h>
#include <boost/algorithm/string.hpp>

namespace cyng {

	column::column(std::string const& name, type_code type)
		: name_(name)
		, type_(type)
	{}

	column_sql::column_sql(column const& col, std::size_t width)
		: column(col)
		, width_(width)
	{}

	column_sql::column_sql(std::string const& name, type_code type, std::size_t width)
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
		std::size_t width_idx{ 0 };
		m.loop([&](std::size_t idx, column const& col, bool pk)->void {
			if (!pk && !gen) {
				//	insert gen
				gen = true;
				vec.push_back(column_sql("gen", TC_UINT64, 0u));
				vec.push_back(column_sql(col, width.at(width_idx++)));
			}
			else {
				if (width_idx < width.size()) {
					vec.push_back(column_sql(col, width.at(width_idx++)));
				}
				else {
					vec.push_back(column_sql(col, 0));
				}
			}
		});

		//	derive SQL table name
		std::string table_name = m.get_name();
		if (!table_name.empty()) {
			table_name.at(0) = std::toupper(table_name.at(0));
		}

		return meta_sql("T" + table_name, std::begin(vec), std::end(vec), m.key_size());
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
