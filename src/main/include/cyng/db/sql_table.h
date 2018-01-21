/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_SQL_TABLE_H
#define CYNG_DB_SQL_TABLE_H

#include <cyng/table/meta_interface.h>
#include <cyng/table/record.h>
#include <cyng/db/interface_result.h>

namespace cyng
{
	/**
	 * Read a result set into an attribute map.
	 */
	attr_map_t to_attr_map(table::meta_table_ptr, db::result_ptr);

	/**
	 * Read a result set into a parameter map.
	 */
	param_map_t to_param_map(table::meta_table_ptr, db::result_ptr);

	/**
	 * Read a result set into an object tuple. All elements
	 * are simple objects.
	 */
	tuple_t to_tuple(table::meta_table_ptr, db::result_ptr);

	/**
	 * Read a result set into a key and and a body
	 * object tuple.
	 */
	table::record to_record(table::meta_table_ptr, db::result_ptr);

}	//	cyng

#endif	//	CYNG_DB_SQL_TABLE_H
