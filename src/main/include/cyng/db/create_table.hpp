/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_SQL_CREATE_TABLE_HPP
#define CYNG_DB_SQL_CREATE_TABLE_HPP

#include <cyng/db/session.h>
#include <cyng/m2m/sql/sql_callback.h> //	custom_callback(cyng::sql::dialect d, std::uint32_t code, std::size_t size)
#include <cyng/sql/sql.h>

namespace cyng
{
	namespace db 
	{
		template< typename T >
		bool create_table(session& s)
		{
			const T tbl;
			auto cmd = cyng::sql::factory(tbl, std::bind(&cyng::m2m::custom_callback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
			cmd->create(s.get_dialect());

			const std::string sql = cmd->to_str();
#ifdef _DEBUG
			std::cout << sql << std::endl;
#endif

			return s.execute(sql);

		}
	}	//	db
}	//	cyng

#endif	//	CYNG_DB_SQL_CREATE_TABLE_HPP
