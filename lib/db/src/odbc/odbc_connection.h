/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_ODBC_CONNECTION_H
#define CYNG_DB_ODBC_CONNECTION_H

#if CYNG_ODBC_INSTALLED == 1
#include "odbc_handle.hpp"

namespace cyng
{
	namespace db	
	{
		namespace odbc	
		{
			class connection	
			{
			public:
				connection();
				virtual ~connection();

				/**
				*	Const conversion operator into reference to native type.
				*/
				operator const SQLHDBC& () const;

				bool is_valid() const;

			private:
				odbc_environment_handle	env_;
				odbc_db_connection_handle dbc_;
			};

		}	//	odbc
	}	//	db
}	// cyng

#endif 	//	CYNG_ODBC_INSTALLED
#endif	//	CYNG_DB_ODBC_CONNECTION_H

