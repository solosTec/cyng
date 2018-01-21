/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_SQLITE_RESULT_H && CYNG_SQLITE3_INSTALLED
#define CYNG_DB_SQLITE_RESULT_H

#include <cyng/db/interface_result.h>
#include "sqlite_statement.h"

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{
			class sqlite_result 
			: public interface_result
			, public std::enable_shared_from_this<sqlite_result>
			{
			public:
				sqlite_result(statement::shared_type);
				
				static result_ptr factory(statement::shared_type);
				
				virtual object get(int, std::size_t, std::size_t /*= SQL_NTS*/) override;
				virtual std::size_t column_count() override;

			private:
				statement::shared_type	statement_;
			};
		}
	}
}

#endif	//	CYNG_DB_SQLITE_RESULT_H
