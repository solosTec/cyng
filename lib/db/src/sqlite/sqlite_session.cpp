/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#include "sqlite_session.h"
#include "sqlite_error.hpp"
#include "sqlite_statement.h"
#include <cyng/db/interface_statement.h>
#include <cyng/dom/algorithm.h>

namespace cyng	
{
	namespace db	
	{
		namespace sqlite	
		{

			session::session(int busy_timeout)
				: connection_()
				, busy_timeout_(busy_timeout)
			{}

			session::~session()
			{
				close();
			}


			std::pair<std::string, bool> session::connect(param_map_t const& config)	
			{
				connection_.close();
				
				//	read database file name from configuration
				const std::string file_name = find_value(config, "file-name", std::string("sqlite.database"));
				const int busy_timeout = find_value(config, "busy-timeout", busy_timeout_);
				
				if (connection_.open(file_name))	
				{
					connection_.set_busy_timeout(busy_timeout);
					connection_.set_update_handler(std::bind(&session::busy_monitor, this, std::placeholders::_1));
					return std::make_pair(file_name, true);
				}
				return std::make_pair(file_name, false);
			}

			bool session::close()	
			{
				release_memory();
				return connection_.close();
			}

			bool session::is_alive() const	
			{
				return connection_.is_open();
			}

			bool session::begin()	
			{
				return is_ok(connection_.execute("BEGIN IMMEDIATE"));
			}

			bool session::commit()	
			{
				return is_ok(connection_.execute("COMMIT"));
			}

			bool session::rollback()	
			{
				return is_ok(connection_.execute("ROLLBACK"));
			}

			bool session::is_transaction() const	
			{
				return connection_.is_transaction();
			}

			boost::system::error_code	session::last_error() const	
			{
				return error::make_error_code(connection_.last_error_code());
			}

			statement_ptr session::create_statement()
			{
				return statement_factory(connection_);
			}
				
			bool session::execute(std::string const& sql)
			{
				const int rc = connection_.execute(sql);
				if (!is_ok(rc))
				{
					connection_.show_diagnostics();
					return false;
				}
				return true;
			}
				
			void session::release_memory() 
			{
				const int to_free(1024);
				int n = 0;
				while ((n = ::sqlite3_release_memory(to_free)) > 0)	
				{
#ifdef _DEBUG
					std::cout
						<< n
						<< " bytes released"
						<< std::endl
						;
#endif
				}
			}

			void session::busy_monitor(int counter) 
			{
#ifdef _DEBUG
				std::cout
					<< "sqlite::busy_monitor( "
					<< counter
					<< " )"
					<< std::endl
					;
#endif
			}
			
			cyng::sql::dialect session::get_dialect() const
			{
				return cyng::sql::SQLITE;
			}

		}	//	sqlite
	}	//	db
}	// cyng

