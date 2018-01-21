/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#if ! defined CYNG_DB_ODBC_RESULT_H && CYNG_ODBC_INSTALLED
#define CYNG_DB_ODBC_RESULT_H

#include <cyng/db/interface_result.h>
#include "odbc_statement.h"

namespace cyng	
{
	namespace db	
	{
		namespace odbc
		{
			class odbc_result
			: public interface_result
			, public std::enable_shared_from_this<odbc_result>
			{
			public:
				odbc_result(statement::shared_type);
				
				static result_ptr factory(statement::shared_type);
				
				virtual object get(int, std::size_t, std::size_t /*= SQL_NTS*/) override;
				virtual std::size_t column_count() override;

			private:
				statement::shared_type	statement_;
			};
		}
	}
}

#endif	//	NODDY_DB_ODBC_RESULT_H
