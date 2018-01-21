/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#include "sqlite_connection.h"
#include "sqlite_error.hpp"
#include <iostream>
#include <sstream>

namespace cyng
{
	namespace db	
	{
		namespace sqlite	
		{
			namespace	//	anonymous namespace
			{	

				//	+-----------------------------------------------------------------+
				//	| busy_handler_impl [function]
				//	+-----------------------------------------------------------------+
				int busy_handler_impl(void* p, int cnt)
				{
					connection::busy_handler* h = static_cast<connection::busy_handler*>(p);
					return (*h)(cnt);
				}

				//	+-----------------------------------------------------------------+
				//	| commit_hook_impl [function]
				//	+-----------------------------------------------------------------+
				int commit_hook_impl(void* p)
				{
					connection::commit_handler* h = static_cast<connection::commit_handler*>(p);
					return (*h)();
				}

				//	+-----------------------------------------------------------------+
				//	| rollback_hook_impl [function]
				//	+-----------------------------------------------------------------+
				void rollback_hook_impl(void* p)
				{
					connection::rollback_handler* h = static_cast<connection::rollback_handler*>(p);
					(*h)();
				}

				//	+-----------------------------------------------------------------+
				//	| update_hook_impl [function]
				//	+-----------------------------------------------------------------+
				void update_hook_impl(void* p, int opcode, char const* dbname, char const* tablename, sqlite3_int64 rowid)
				{
					connection::update_handler* h = static_cast<connection::update_handler*>(p);
					(*h)(opcode, dbname, tablename, rowid);
				}

				//	+-----------------------------------------------------------------+
				//	| authorizer_impl [function]
				//	+-----------------------------------------------------------------+
				int authorizer_impl(void* p, int evcode, char const* p1, char const* p2, char const* dbname, char const* tvname)
				{
					connection::authorize_handler* h = static_cast<connection::authorize_handler*>(p);
					return (*h)(evcode, p1, p2, dbname, tvname);
				}

			} // end of anonymous namespace

			connection::connection()
				: dbp_(nullptr)
				, busy_handler_()
				, commit_handler_()
				, rollback_handler_()
				, update_handler_()
				, authorize_handler_()
			{
				if (!is_ok(::sqlite3_initialize()))
				{
					show_diagnostics();
				}
			}

			connection::~connection()	
			{
				close();
				if (!is_ok(::sqlite3_shutdown()))
				{
					show_diagnostics();
				}
			}

			bool connection::is_open() const	
			{
				return dbp_ != nullptr;
			}

			bool connection::open(std::string const& filename )	
			{
				int rc = ::sqlite3_open(filename.c_str(), &dbp_);
//				std::cout << "dbp_: " << (long)dbp_ << std::endl;
				return 	(is_ok(rc) && is_open());
			}

			bool connection::close()	
			{
				if (is_open())	
				{
					const int rc = ::sqlite3_close(dbp_);
					dbp_ = NULL;
					return is_ok(rc);
				}
				return false;
			}

			int connection::last_error_code() const	
			{
				if (is_open())	
				{
					return ::sqlite3_errcode(dbp_);
				}
				return error::SQLite_ERROR;
			}

			std::string connection::last_error_msg() const	
			{
				if (is_open())	
				{
					const std::string msg = ::sqlite3_errmsg(dbp_);
					return msg;
				}
				return "DB closed";
			}

			void connection::show_diagnostics() const 
			{
				std::stringstream ss;
				ss
					<< '['
					<< last_error_code()
					<< ']'
					<< ' '
					<< last_error_msg()
					;
//#ifdef _WIN32
//				::OutputDebugString(ss.str().c_str());
//#endif
				std::cerr
					<< ss.str()
					<< std::endl
					;

			}


			int connection::execute(std::string const& sql)		
			{
				return ::sqlite3_exec(dbp_, sql.c_str(), 0, 0, 0);
			}

			bool connection::set_busy_timeout(int ms)	
			{
				return is_ok(::sqlite3_busy_timeout(dbp_, ms));
			}

			void connection::set_busy_handler(busy_handler h)	
			{
				busy_handler_.swap(h);
				::sqlite3_busy_handler(dbp_
					, busy_handler_ ? busy_handler_impl : 0
					, &busy_handler_);
			}

			void connection::set_commit_handler(commit_handler h)	
			{
				commit_handler_.swap(h);
				::sqlite3_commit_hook(dbp_
					, commit_handler_ ? commit_hook_impl : 0
					, &commit_handler_);
			}

			void connection::set_rollback_handler(rollback_handler h)	
			{
				rollback_handler_.swap(h);
				::sqlite3_rollback_hook(dbp_
					, rollback_handler_ ? rollback_hook_impl : 0
					, &rollback_handler_);
			}

			void connection::set_update_handler(update_handler h)	{
				update_handler_.swap(h);
				::sqlite3_update_hook(dbp_
					, update_handler_ ? update_hook_impl : 0
					, &update_handler_);
			}

			void connection::set_authorize_handler(authorize_handler h)	{
				authorize_handler_.swap(h);
				::sqlite3_set_authorizer(dbp_
					, authorize_handler_ ? authorizer_impl : 0
					, &authorize_handler_);
			}

			sqlite3_int64 connection::last_insert_rowid() const	
			{
				return ::sqlite3_last_insert_rowid(dbp_);
			}

			bool connection::is_transaction() const
			{
				return ::sqlite3_get_autocommit(dbp_) == 0;
			}
				
			connection::operator sqlite3*()	{
				return dbp_;
			}


		}	//	sqlite
	}	//	db
}	// cyng


