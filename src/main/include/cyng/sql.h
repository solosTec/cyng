/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#ifndef CYNG_SQL_H
#define CYNG_SQL_H

#include <cyng/table/meta_interface.h>
#include <cyng/sql/dialect.h>
#include <sstream>

namespace cyng 
{
	using namespace table;
	namespace sql
	{
		
		class command;
		class sql_select;
// 		class sql_select_all;
// 		class sql_select_distinct;
// 		class sql_select_count;
		class sql_from;
		class sql_where;
 		class sql_group;
// 		class sql_having;
 		class sql_order;
		class sql_update;
		class sql_insert;
		class sql_create;
 		class sql_remove;	//	SQL delete
// 		class sql_drop;
		
		class base
		{
		public:
			base(meta_table_ptr, dialect, std::ostream&);
			base(base&) = default;

			/**
			 * return true if meta data available 
			 */
			bool is_valid() const;

			meta_table_ptr get_meta() const;

		protected:
			bool do_skip(std::string) const;

		protected:
			/**
			 * Provide table description
			 */
			meta_table_ptr meta_;

			/**
			 * target SQL type
			 */
			dialect const dialect_;

			/**
			 * contains the SQL statement
			 */
			std::ostream& stream_;
		};

		/**
		 * CREATE ...
		 */
		class sql_create : public base
		{
		public:
			sql_create(meta_table_ptr, dialect, std::ostream&);
			
		private:
			void write_columns();
			void write_pks();
			bool has_pk() const;
		};

		/**
		 * ... ORDER BY ...
		 */
		class sql_order : public base
		{
		public:
			sql_order(meta_table_ptr, dialect, std::ostream&);
		};

		/**
		 * ... GROUP BY ...
		 */
		class sql_group : public base
		{
		public:
			sql_group(meta_table_ptr, dialect, std::ostream&);
			sql_order order_by(std::string const& term);
		};

		/**
		 * ... WHERE ...
		 */
		class sql_where : public base
		{
		public:
			sql_where(meta_table_ptr, dialect, std::ostream&);
			
			template < typename EXPR >
			sql_group group_by(EXPR const& expr)
			{
				stream_
					<< "GROUP BY "
					;
				expr.serialize(stream_, meta_, dialect_, false);
				return sql_group(meta_, dialect_, stream_);
			}

			sql_order order_by(std::string const& term);
		};

		class sql_from : public base
		{
		public:
			sql_from(meta_table_ptr, dialect, std::ostream&);
			
			template < typename EXPR >
			sql_where where(EXPR const& expr)
			{
				stream_ 
				<< "WHERE "
				;
				expr.serialize(stream_, meta_, dialect_, true);
				stream_ << ' ';
				return sql_where(meta_, dialect_, stream_);
			}
		};
		
		/**
		 * SELECT ...
		 */
		class sql_select : public base
		{
		public:
			sql_select(meta_table_ptr, dialect, std::ostream&);

			/**
			 * use expression templates to build 
			 * an SQL expression.
			 */
			template < typename LIST >
			sql_from operator[](LIST const& list)
			{
 				list.serialize(stream_, meta_, dialect_, false);
				stream_ << ' ';
 				return sql_from(meta_, dialect_, stream_);
			}
			
			sql_from all();


			//
			//	aggregate functions
			//
 			void count(std::size_t);
 			void sum(std::size_t);
 			void avg(std::size_t);
			
			/**
			 * inner join of two tables over all columns.
			 * ToDo: implement ON function on result.
			 */
			sql_from inner_join_all_on_pk(meta_table_ptr);
			
		private:
			void write_columns(meta_table_ptr, bool& init_flag);
			void write_pk(meta_table_ptr, meta_table_ptr);
		};
		
		/**
		 * INSERT INTO ...
		 */
		class sql_insert : public base
		{
		public:
			sql_insert(meta_table_ptr, dialect, std::ostream&);
			
		private:
			void write_columns();
 			void write_placholders();
		};
		
		/**
		 * UPDATE ...
		 */
		class sql_update : public base
		{
		public:
			sql_update(meta_table_ptr, dialect, std::ostream&);
			
			template < typename EXPR >
			sql_where where(EXPR const& expr)
			{
				stream_ 
				<< "WHERE "
				;
				expr.serialize(stream_, meta_, dialect_, false);
				return sql_where(meta_, dialect_, stream_);
			}

			sql_where by_key();
		};

		/**
		 * DELETE ...
		 */
		class sql_remove : public base
		{
		public:
			sql_remove(meta_table_ptr, dialect, std::ostream&);
			
			template < typename EXPR >
			sql_where where(EXPR const& expr)
			{
				stream_
					<< "WHERE "
					;
				expr.serialize(stream_, meta_, dialect_, false);
				return sql_where(meta_, dialect_, stream_);
			}

			sql_where by_key();
		};


		/**
		 * Generate SQL commands for a single table.
		 */
		class command 
		{
		public:
			command(meta_table_ptr, dialect);
			
			/**
			 * Create a select clause.
			 */
			sql_select select();
			
			/**
			 * Assemble the SQL create statement for this table.
			 */
			sql_create create();
			
			/**
			 * Assemble the SQL delete statement for this table.
			 */
			sql_remove remove();

			/**
			 * Assemble the SQL insert statement for this table.
			 */
			sql_insert insert();

			/**
			 * Assemble the SQL update statement for this table.
			 */
			template < typename LIST >
			sql_update update(LIST const& list)
			{
				clear("UPDATE");
				
				stream_
				<< meta_->get_name()
				<< " SET "
				;
				
 				list.serialize(stream_, meta_, dialect_, false);
				stream_ << ' ';
				
 				return sql_update(meta_, dialect_, stream_);
			}

			/**
			 * @return assembled SQL command 
			 */
			std::string to_str() const;

			/**
			 * return true if meta data available
			 */
			bool is_valid() const;

			meta_table_ptr get_meta() const;

			
		private:
			void clear();
			void clear(std::string const&);
			
		private:
			/**
			 * Provide table description 
			 */
			meta_table_ptr meta_;
			
			/**
			 * target SQL type 
			 */
			dialect const dialect_;
			
			/**
			 * contains the SQL statement 
			 */
			std::stringstream stream_;
		};
	}	
}

#endif	//	CYNG_SQL_H

