/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/

#ifndef CYNG_DB_SQL_TABLE_HPP
#define CYNG_DB_SQL_TABLE_HPP

#include <cyng/db/sql_table_base.h>
#include <cyng/util/read_sets.h>
#include <cyng/intrinsics/type_traits.h>

namespace cyng
{
	namespace db 
	{

		template< typename ...Args >
		class sql_table
			: public sql_table_base
		{
		public:
			//	support for typesafe reading and writing
			typedef typename cyy::writer_factory< Args ...>	writer_t;
			typedef typename writer_t::return_type			tuple_type;
			typedef sql_table<Args...>	sql_table_type;

		public:
			/**
			 * @param name table name
			 */
			sql_table(std::string const& name)
				: sql_table_base(name)
			{}

			/**
			 * add a primary to table schema
			 *
			 * @param name column name
			 * @param type data type encodes with cyy type code
			 * @param with column width (optional, required for strings and blob fields)
			 * @param nullable if true column accepts NULL values
			 * @param def default value (the syntax of this value must be chosen according to the
			 * data type of the database and not the specified cyy type code. e.g. A UUID value is stored
			 * as string on most databases.)
			 */
			std::size_t add_pk(std::string const& name,
				std::size_t width = -1, 
				bool nullable = false, 
				std::string const& def = "")
			{
				//	get type information from factory
				const std::size_t idx = size();
				BOOST_ASSERT_MSG(idx < std::tuple_size<tuple_type>::value, "index out of range");
				const cyy::code_t code = cyy::get_type_code<tuple_type>()(idx);
				return add_column(name, code, true, width, nullable, def);
			}

			/**
			 * add a column to table schema
			 *
			 * @param name column name
			 * @param type data type encodes with cyy type code
			 * @param with column width (optional, required for strings and blob fields)
			 * @param nullable if true column accepts NULL values
			 * @param def default value (the syntax of this value must be chosen according to the
			 * data type of the database and not the specified cyy type code. e.g. A UUID value is stored
			 * as string on most databases.)
			 */
			std::size_t add_col(std::string const& name,
				std::size_t width = -1,
				bool nullable = false,
				std::string const& def = "")
			{
				const std::size_t idx = size();
				BOOST_ASSERT_MSG(idx < std::tuple_size<tuple_type>::value, "index out of range");
				const cyy::code_t code = cyy::get_type_code<tuple_type>()(idx);
				return add_column(name, code, false, width, nullable, def);
			}

			/**
			 * @return a tuple with all unzipped data from the
			 * result set.
			 */
			tuple_type unzip(result_ptr ptr) const
			{
				return (ptr)
					? writer_t::write(read_as_tuple(ptr))
					: tuple_type()
					;
			}
			

		};
	}	//	db
}	//	cyng

#endif	//	CYNG_DB_SQL_TABLE_HPP
