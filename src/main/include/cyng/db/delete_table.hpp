/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_SQL_DELETE_TABLE_HPP
#define CYNG_DB_SQL_DELETE_TABLE_HPP

#include <cyng/db/session.h>
#include <cyng/sql/sql.h>

namespace cyng
{
	namespace db 
	{
		template< typename T >
		bool delete_table(session& s)
		{
			const T tbl;
			auto cmd = cyng::sql::factory(tbl, cyng::sql::custom_callback());
			cmd->remove(s.get_dialect());

			const std::string sql = cmd->to_str();
#ifdef _DEBUG
			std::cout << sql << std::endl;
#endif

			return s.execute(sql);

		}
	}	//	db
}	//	cyng

#endif	//	CYNG_DB_SQL_DELETE_TABLE_HPP
