/*
* The MIT License (MIT)
*
* Copyright (c) 2021 Sylko Olzscher
*
*/


#include <cyng.h>
#include <cyng/db/session.h>
#include <cyng/obj/container_cast.hpp>
#include <cyng/obj/util.hpp>
#include <cyng/obj/algorithm/find.h>

#ifdef _DEBUG_DB
#include <iostream>
#endif

#if defined(CYNG_MYSQL_CONNECTOR)
//#include "mysql/mysql_session.h"
#endif
			
#if defined(CYNG_ODBC_CONNECTOR)
#include <odbc/odbc_session.h>
#endif

#if defined(CYNG_PQXX_CONNECTOR)
#endif


#if defined(CYNG_SQLITE3_CONNECTOR)
#include <sqlite3/sqlite_session.h>
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
 			return (connection_)
				? connection_->is_alive()
				: false
				;
		}

		bool session::close()	
		{
			return (connection_)
				? connection_->close()
				: false
				;
		}
	
		bool session::begin()	
		{
			return (connection_)
				? connection_->begin()
				: false
				;
		}

		bool session::commit()	
		{
			return (connection_)
				? connection_->commit()
				: false
				;
		}

		bool session::rollback()	
		{
			return (connection_)
				? connection_->rollback()
				: false
				;
		}

		bool session::is_transaction() const	
		{
			return (connection_)
				? connection_->is_transaction()
				: false
				;
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
			switch (type)
			{
			
#if defined(CYNG_MYSQL_CONNECTOR)
			//case connection_type::MYSQL:	return std::make_shared<mysql::session>();
#endif
			
#if defined(CYNG_SQLITE3_CONNECTOR)
			case connection_type::SQLITE:	return std::make_shared<sqlite::session>();
#endif
			
#if defined(CYNG_ODBC_CONNECTOR)
			//case connection_type::ODBC:	return std::make_shared<odbc::session>();
#endif

#if defined(CYNG_PQXX_CONNECTOR)
			//case connection_type::PQXX:	return std::make_shared<pqxx::session>();
#endif
				default:
					break;
			}
 			return session_ptr();
		}
		
		cyng::sql::dialect session::get_dialect() const
		{
 			return (connection_)
				? connection_->get_dialect()
				: cyng::sql::dialect::GENERIC
				;
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

		session create_db_session(param_map_t const& pmap) {
			auto const ct = to_connection_type(find_value(pmap, std::string("connection-type"), std::string("none")));
			session s(ct);
			auto r = s.connect(pmap);
			if (!r.second) {
#ifdef _DEBUG_DB
				std::cerr
					<< "**error: cannot open connection "
					<< r.first
					<< std::endl
					;
#endif
			}
			return s;
		}

		session create_db_session(cyng::object const& cfg) {

			if (is_of_type<TC_TUPLE>(cfg)) {
				auto const tpl = container_cast<tuple_t>(cfg);
				auto const pmap = to_param_map(tpl);
				return create_db_session(pmap);
			}
			else if (is_of_type<TC_PARAM_MAP>(cfg)) {
				auto const pmap = container_cast<param_map_t>(cfg);
				return create_db_session(pmap);
			}
			return session(connection_type::NONE);
		}

	}
}

