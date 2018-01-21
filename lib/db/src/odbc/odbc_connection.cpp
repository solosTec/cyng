/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include "odbc_connection.h"

namespace cyng	
{
	namespace db	
	{
		namespace odbc	
		{

			connection::connection()
				: env_()
				, dbc_(env_.get_handle())
			{}

			connection::~connection()	
			{
				::SQLDisconnect(dbc_);
				//	handles should be freed automatically
				//	force a special order
				//dbc_.~odbc_handle();
				//env_.~odbc_handle();
			}

			connection::operator const SQLHDBC& () const	
			{
				return dbc_.get_handle();
			}

			bool connection::is_valid() const	
			{
				return env_.is_valid() && dbc_.is_valid();
			}

		}	//	odbc
	}	//	db
}	// cyng


