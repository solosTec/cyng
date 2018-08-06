
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#include "test-db-002.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/db/session_pool.h>
#include <cyng/db/interface_statement.h>
#include <cyng/async/scheduler.h>
#include <cyng/factory.h>

#include <cyng/sql.h>
#include <cyng/table/meta.hpp>
#include <cyng/sql/dsl/binary_expr.hpp>
#include <cyng/sql/dsl/list_expr.hpp>
#include <cyng/sql/dsl/operators.hpp>
#include <cyng/sql/dsl/constant.hpp>
#include <cyng/sql/dsl/variable.hpp>
#include <cyng/sql/dsl/column.h>
#include <cyng/sql/dsl/placeholder.h>
#include <cyng/sql/dsl/aggregate.h>

namespace cyng 
{

	bool test_db_002()
	{
#if CYNG_ODBC_INSTALLED == 1
		async::scheduler scheduler;
		db::session_pool pool(scheduler.get_io_service(), db::CONNECTION_ODBC);
		param_map_t cfg;
		cfg["connect"] = make_object("DSN=cyng;Uid=root;Pwd=melissa;AUTO_RECONNECT=0;");
		cfg["login-timeout"] = make_object(60);
		cfg["connect-timeout"] = make_object(60);
		cfg["dialect"] = make_object("MYSQL");
		pool.start(1, std::chrono::seconds(0), cfg);
		auto session = pool.get_session();

		auto m1_ptr = table::make_meta_table<1, 2>("employees", { "id", "name", "age" }, { TC_UINT32, TC_STRING, TC_TIME_POINT }, { 1, 32, 1 });
		sql::command cmd(m1_ptr, sql::MYSQL);
		cmd.create();
		std::string sql = cmd.to_str();
		std::cout << sql << std::endl;
		//	CREATE TABLE IF NOT EXISTS employees (id INT UNSIGNED, name VARCHAR(32), age DATETIME, PRIMARY KEY(id, name))
		BOOST_CHECK_EQUAL(sql, "CREATE TABLE IF NOT EXISTS employees (id INT UNSIGNED, name VARCHAR(32), age DATETIME, PRIMARY KEY(id, name))");
		session.execute(sql);

		cmd.insert();
		sql = cmd.to_str();
		//std::cout << sql << std::endl;
		//	INSERT INTO employees (id, name, age) VALUES (?, ?, ?)
		BOOST_CHECK_EQUAL(sql, "INSERT INTO employees (id, name, age) VALUES (?, ?, ?)");

		//	prepare an insert statement
		auto stmt = session.create_statement();
		std::pair<int, bool> r = stmt->prepare(sql);
		BOOST_CHECK_EQUAL(r.first, 3);	//	3 parameters to bind
		BOOST_CHECK(r.second);

		//	bind parameters
		stmt->push(make_object(42), 0).push(make_object("Douglas Adams"), 32).push(make_object(std::chrono::system_clock::now()), 0);
		stmt->execute();
		stmt->clear();

		//	prepare a delete statement
		cmd.remove().where(sql::column(1) == sql::make_constant(42));
		sql = cmd.to_str();

		r = stmt->prepare(sql);
		BOOST_CHECK_EQUAL(r.first, 0);	//	0 parameters to bind
		BOOST_CHECK(r.second);
		stmt->execute();
		stmt->clear();

		pool.stop();
		scheduler.stop();
#endif
		return true;
	}
	
}


