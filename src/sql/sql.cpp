/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher
 *
 */ 

#include <cyng/sql/sql.hpp>

namespace cyng
{
	namespace sql {

		select::select(dialect d)
			: dialect_(d)
		{}
	}
}

