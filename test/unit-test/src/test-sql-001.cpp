
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
		//std::cout << traits::get_type_name(dbmtp->get_type(0)) << std::endl;
		//std::cout << traits::get_type_name(dbmtp->get_type(1)) << std::endl;
		//std::cout << traits::get_type_name(dbmtp->get_type(2)) << std::endl;
		BOOST_CHECK_EQUAL(dbmtp->get_type(0), TC_UINT32);
		BOOST_CHECK_EQUAL(dbmtp->get_type(1), TC_STRING);
		BOOST_CHECK_EQUAL(dbmtp->get_type(2), TC_TIME_POINT);

		cmd.select()[sql::column(2)];
// 		std::cout << "'" << cmd.to_str() << "'" << std::endl;
		//BOOST_CHECK_EQUAL(cmd.to_str(), "SELECT name FROM employees ");

		//cmd.select()[sql::constant<int>(1), sql::column(2)].where(sql::column(2) == sql::make_placeholder());
		//std::cout << cmd.to_str() << std::endl;

		cmd.create();
// 		std::cout << cmd.to_str() << std::endl;
		BOOST_CHECK_EQUAL(cmd.to_str(), "CREATE TABLE IF NOT EXISTS employees (id INT, name TEXT, age FLOAT, PRIMARY KEY(id, name))");

		cmd.insert();
		//std::cout << cmd.to_str() << std::endl;
		BOOST_CHECK_EQUAL(cmd.to_str(), "INSERT INTO employees (id, name, age) VALUES (?, ?, julianday(?))");

		//sql::make_assign(2, sql::make_constant("hello"));
		cmd.update(sql::make_assign(2, sql::make_constant("hello"))).where(sql::column(1) == sql::make_constant(42) && sql::column(2) == sql::make_constant("dummy"));
		//std::cout << cmd.to_str() << std::endl;
		BOOST_CHECK_EQUAL(cmd.to_str(), "UPDATE employees SET name = 'hello' WHERE ((id = 42) AND (name = 'dummy'))");
		cmd.update(sql::make_assign(2, sql::make_placeholder())).where(sql::column(1) == sql::make_constant(42) && sql::column(2) == sql::make_constant("dummy"));
		//std::cout << cmd.to_str() << std::endl;
		BOOST_CHECK_EQUAL(cmd.to_str(), "UPDATE employees SET name = ? WHERE ((id = 42) AND (name = 'dummy'))");

		cmd.update(sql::make_list(sql::make_assign(2, sql::make_constant("hello"))
		, sql::make_assign(3, sql::make_placeholder())));
		//std::cout << '[' << cmd.to_str() << ']' << std::endl;
		BOOST_CHECK_EQUAL(cmd.to_str(), "UPDATE employees SET name = 'hello', datetime(age) = ? ");

		std::string emp("alfred");
		cmd.remove().where(sql::column(2) == sql::make_variable(emp));
		//std::cout << cmd.to_str() << std::endl;
		BOOST_CHECK_EQUAL(cmd.to_str(), "DELETE FROM employees WHERE (name = 'alfred')");

		return true;
	}
	
}


