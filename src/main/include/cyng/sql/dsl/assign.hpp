/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_DSL_ASSIGN_H
#define CYNG_SQL_DSL_ASSIGN_H

#include <ostream>
#include <cyng/table/meta_interface.h>
#include <cyng/sql/dialect.h>
#include <cyng/sql/dsl/constant.hpp>
#include <cyng/sql/dsl/variable.hpp>
#include <cyng/sql/dsl/placeholder.h>
#include <cyng/sql/dsl/column.h>

namespace cyng 
{
	using namespace table;
	namespace sql
	{
		/**
		 * Convert a columns index into a column name.
		 * The index starts with one.
		 */
		template < typename T >
		struct assign
		{
			assign(std::size_t index, T const& val)
				: col_(index)
				, val_(val)
			{}

			void serialize(std::ostream& os, meta_table_ptr tbl, dialect dia, bool lhe) const
			{
				col_.serialize(os, tbl, dia, true);
				os << " = ";
				val_.serialize(os, tbl, dia, false);
			}
			
			const column col_;
			const T val_;
		};
		
		template < typename T >
		auto make_assign(std::size_t index, constant<T> c) -> assign<constant<T>>
		{
			using A = assign<constant<T>>;
			return A(index, c);
		}

		template < typename T >
		auto make_assign(std::size_t index, variable<T> v) -> assign<variable<T>>
		{
			using A = assign<variable<T>>;
			return A(index, v);
		}

		auto make_assign(std::size_t index, placeholder p) -> assign< placeholder >
		{
			using A = assign<placeholder>;
			return A(index, p);
		}

	}
}

#endif	//	CYNG_SQL_DSL_ASSIGN_H





