/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_ODBC_CONNECTION_H
#define CYNG_DB_ODBC_CONNECTION_H

#include <cyng.h>

#if defined(CYNG_ODBC_CONNECTOR)

#include <odbc/odbc_handle.hpp>

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

#endif
#endif

