
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
#include <cyng/store/meta.hpp>
#include <cyng/sql/dsl/binary_expr.hpp>
#include <cyng/sql/dsl/list_expr.hpp>
#include <cyng/sql/dsl/operators.hpp>
#include <cyng/sql/dsl/constant.hpp>
#include <cyng/sql/dsl/variable.hpp>
#include <cyng/sql/dsl/column.h>
#include <cyng/sql/dsl/placeholder.h>
#include <cyng/sql/dsl/aggregate.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/intrinsics/traits/tag_names.hpp>

namespace cyng 
{

	bool test_sql_001()
	{
		auto dbmtp = store::make_meta_table<1, 2>("employees", {"id", "name", "age"}, {TC_UINT32, TC_STRING, TC_TIME_POINT}, {1, 32, 1});
		sql::command cmd(dbmtp, sql::MYSQL);
// 		cmd.do_select();
// 		std::cout << cmd.to_str() << std::endl;
		std::cout << traits::get_type_name(dbmtp->get_type(0)) << std::endl;
		std::cout << traits::get_type_name(dbmtp->get_type(1)) << std::endl;
		std::cout << traits::get_type_name(dbmtp->get_type(2)) << std::endl;
		
		cmd.select()[sql::column(2)];
		std::cout << cmd.to_str() << std::endl;

		cmd.select()[sql::constant<int>(1), sql::column(2)].where(sql::column(2) == sql::make_placeholder());
		std::cout << cmd.to_str() << std::endl;
		
		cmd.create();
		std::cout << cmd.to_str() << std::endl;
		
		cmd.insert();
		std::cout << cmd.to_str() << std::endl;
		
		cmd.update(sql::column(2)).where(sql::column(1) == sql::constant<int>(42));
		std::cout << cmd.to_str() << std::endl;
		
		return true;
	}
	
}


