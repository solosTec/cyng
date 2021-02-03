#include <cyng/store/meta.h>


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

}
