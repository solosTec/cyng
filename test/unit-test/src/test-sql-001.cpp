
/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include "test-sql-001.h"
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <cyng/sql.h>
#include <cyng/table/meta.hpp>
#include <cyng/sql/dsl/binary_expr.hpp>
#include <cyng/sql/dsl/list_expr.hpp>
#include <cyng/sql/dsl/operators.hpp>
//#include <cyng/sql/dsl/constant.hpp>
//#include <cyng/sql/dsl/variable.hpp>
//#include <cyng/sql/dsl/column.h>
//#include <cyng/sql/dsl/placeholder.h>
#include <cyng/sql/dsl/aggregate.h>
#include <cyng/sql/dsl/assign.hpp>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/intrinsics/traits/tag_names.hpp>

namespace cyng 
{

	bool test_sql_001()
	{
		auto dbmtp = table::make_meta_table<2, 1>("employees", {"id", "name", "age"}, {TC_UINT32, TC_STRING, TC_TIME_POINT}, {1, 32, 1});
		//sql::command cmd(dbmtp, sql::MYSQL);
		sql::command cmd(dbmtp, sql::SQLITE);
		BOOST_CHECK_EQUAL(dbmtp->get_type(0), TC_UINT32);
		BOOST_CHECK_EQUAL(dbmtp->get_type(1), TC_STRING);
		BOOST_CHECK_EQUAL(dbmtp->get_type(2), TC_TIME_POINT);

		auto sql = cmd.select()[sql::column(2)]();
		BOOST_CHECK_EQUAL(sql, "SELECT name FROM employees ");

		//cmd.select()[sql::constant<int>(1), sql::column(2)].where(sql::column(2) == sql::make_placeholder());
		//std::cout << cmd.to_str() << std::endl;

		sql = cmd.create().to_str();
		BOOST_CHECK_EQUAL(sql, "CREATE TABLE IF NOT EXISTS employees (id INT, name TEXT, age FLOAT, PRIMARY KEY(id, name))");

		sql = cmd.insert()();
		BOOST_CHECK_EQUAL(sql, "INSERT INTO employees (id, name, age) VALUES (?, ?, julianday(?))");

		//sql::make_assign(2, sql::make_constant("hello"));
		sql = cmd.update(sql::make_assign(2, sql::make_constant("hello"))).where(sql::column(1) == sql::make_constant(42) && sql::column(2) == sql::make_constant("dummy"))();
		BOOST_CHECK_EQUAL(sql, "UPDATE employees SET name = 'hello' WHERE ((id = 42) AND (name = 'dummy'))");

		sql = cmd.update(sql::make_assign(2, sql::make_placeholder())).where(sql::column(1) == sql::make_constant(42) && sql::column(2) == sql::make_constant("dummy"))();
		BOOST_CHECK_EQUAL(sql, "UPDATE employees SET name = ? WHERE ((id = 42) AND (name = 'dummy'))");

		sql = cmd.update(sql::make_list(sql::make_assign(2, sql::make_constant("hello")), sql::make_assign(3, sql::make_placeholder())))();
		BOOST_CHECK_EQUAL(sql, "UPDATE employees SET name = 'hello', age = julianday(?) ");

		std::string emp("alfred");
		sql = cmd.remove().where(sql::column(2) == sql::make_variable(emp))();
		BOOST_CHECK_EQUAL(sql, "DELETE FROM employees WHERE (name = 'alfred')");

		return true;
	}
	
}


