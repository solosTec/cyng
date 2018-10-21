/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#include <cyng/db/session.h>
#include <cyng/db/interface_session.h>
#include <cyng/set_cast.h>
#include "session_wrapper.hpp"

#if CYNG_MYSQL_INSTALLED == 1
#include "mysql/mysql_session.h"
#endif
			
#if defined(CYNG_SQLITE3_INSTALLED)
#include "sqlite/sqlite_session.h"
#endif
			
#if defined(CYNG_ODBC_INSTALLED)
#include "odbc/odbc_session.h"
#endif

#if CYNG_OLEDB_INSTALLED == 1
#include "oldedb/oledb_session.h"
#endif

namespace cyng
{
	namespace db 
	{	
		session::session(connection_type type)
 		: connection_(reset(type))
		{}

		session::session(session const& other)
		: connection_(other.connection_)
		{}

		session::session(session&& other)
		: connection_(std::move(other.connection_))
		{}
	
		session::~session()
		{}
	
		session& session::operator=(session && other)
		{
			connection_ = std::move(other.connection_);
			return *this;
		}

		std::pair<std::string, bool> session::connect(param_map_t const& config)	
		{
			return (!connection_)
				? std::make_pair(std::string("unknown connection type"), false)
				: connection_->connect(config)
				;
		}

		std::pair<std::string, bool> session::connect(tuple_t const& config)
		{
			param_map_t pmap = to_param_map(config);
			BOOST_ASSERT_MSG(!pmap.empty(), "missing database connection arguments");
			return connect(pmap);
		}

	
		bool session::is_alive() const	
		{
 			return connection_->is_alive();
		}

		bool session::close()	
		{
 			return connection_->close();
		}
	
		bool session::begin()	
		{
 			return connection_->begin();
		}

		bool session::commit()	
		{
 			return connection_->commit();
		}

		bool session::rollback()	
		{
 			return connection_->rollback();
		}

		bool session::is_transaction() const	
		{
 			return connection_->is_transaction();
		}

		boost::system::error_code session::last_error() const	
		{
 			return connection_->last_error();
		}
	
		statement_ptr session::create_statement()	
		{
 			return connection_->create_statement();
		}
		
		bool session::execute(std::string const& sql)
		{
 			return connection_->execute(sql);
		}

		session_ptr session::reset(connection_type type)
		{
#if (CYNG_MYSQL_INSTALLED == 1) || defined(CYNG_SQLITE3_INSTALLED) || defined(CYNG_ODBC_INSTALLED) || (CYNG_OLEDB_INSTALLED == 1)
			switch (type)
			{
			
#if CYNG_MYSQL_INSTALLED == 1
				case CONNECTION_MYSQL:	return session_wrapper< mysql::session >::factory();
#endif
			
#if defined(CYNG_SQLITE3_INSTALLED)
				case CONNECTION_SQLITE:	return session_wrapper< sqlite::session >::factory();
#endif
			
#if defined(CYNG_ODBC_INSTALLED)
				case CONNECTION_ODBC:	return session_wrapper< odbc::session >::factory();
#endif

#if CYNG_OLEDB_INSTALLED == 1
				case CONNECTION_OLEDB:	return session_wrapper< oledb::session >::factory();
#endif
				default:
					break;
			}
#endif
 			return session_ptr();
		}
		
		cyng::sql::dialect session::get_dialect() const
		{
 			return connection_->get_dialect();
		}
		
		transaction::transaction(session& dbs)
			: db_(dbs)
			, abort_(false)
		{
			db_.begin();
		}

		transaction::~transaction()
		{
			if (abort_)		
			{
				db_.rollback();
			}
			else	
			{
				db_.commit();
			}
		}

		void transaction::rollback()
		{
			abort_ = true;
		}
	}
}

