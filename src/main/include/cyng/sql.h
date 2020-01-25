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

			/**
			 * return true if meta data available 
			 */
			bool is_valid() const;

			meta_table_ptr get_meta() const;

			/**
			 * @return constructed SQL command
			 */
			std::string to_str() const;

			/** @brief same as to_str()
			 * 
			 * @return constructed SQL command
			 */
			std::string operator()() const;

		protected:
			/**
			 * Don't create instances of this class
			 */
			base(meta_table_ptr, dialect, std::stringstream&&);
			base(base&&) = default;

			bool do_skip(std::string) const;

			/**
			 * @return true if column is of type time point and
			 * database lacks support for this data type (SQLite).
			 */
			bool is_tp(column const& col) const;

			std::string get_placeholder(column const& col) const;
			std::string get_full_col_name(column const& col) const;

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
			std::stringstream stream_;
		};

		/**
		 * CREATE ...
		 */
		class sql_create : public base
		{
			friend class command;
			sql_create(meta_table_ptr, dialect, std::stringstream&&);

		public:
			/*
			 * satisfy gcc < 5.5
			 */
#if __cplusplus < 201703L			
			sql_create(sql_create&&);
#endif

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
			friend class sql_group;
			friend class sql_where;
			sql_order(meta_table_ptr, dialect, std::stringstream&&);

		public:
			
#if __cplusplus < 201703L
			sql_order(sql_order&&);
#endif
			
		};

		/**
		 * ... GROUP BY ...
		 */
		class sql_group : public base
		{
			friend class sql_where;
			sql_group(meta_table_ptr, dialect, std::stringstream&&);

		public:
			sql_order order_by(std::string const& term);
		};

		/**
		 * ... WHERE ...
		 */
		class sql_where : public base
		{
			friend class sql_from;
			friend class sql_update;
			friend class sql_remove;
			sql_where(meta_table_ptr, dialect, std::stringstream&&);

			/*
			 * satisfy gcc < 5.5
			 */
#if __cplusplus < 201703L
			sql_where(sql_where&&);
#endif

		public:		
			template < typename EXPR >
			sql_group group_by(EXPR const& expr)
			{
				stream_
					<< "GROUP BY "
					;
				expr.serialize(stream_, meta_, dialect_, false);
				return sql_group(meta_, dialect_, std::move(stream_));
			}

			sql_order order_by(std::string const& term);
		};

		class sql_from : public base
		{
			friend class sql_select;
			sql_from(meta_table_ptr, dialect, std::stringstream&&);

			/*
			 * satisfy gcc < 5.5
			 */
#if __cplusplus < 201703L
			sql_from(sql_from&&);
#endif

		public:			
			template < typename EXPR >
			sql_where where(EXPR const& expr)
			{
				stream_ 
				<< "WHERE "
				;
				expr.serialize(stream_, meta_, dialect_, true);
				stream_ << ' ';
				return sql_where(meta_, dialect_, std::move(stream_));
			}

			/**
			 * WHERE clause as primary key
			 */
			sql_where by_key();

		};
		
		/**
		 * SELECT ...
		 */
		class sql_select : public base
		{
		public:
			sql_select(meta_table_ptr, dialect, std::stringstream&&);
			
			/*
			 * satisfy gcc < 5.5
			 */
#if __cplusplus < 201703L 
			sql_select(sql_select&&);
#endif
			
			/**
			 * use expression templates to build 
			 * an SQL expression.
			 */
			template < typename LIST >
			sql_from operator[](LIST const& list)
			{
 				list.serialize(stream_, meta_, dialect_, false);
				stream_ << ' ';
 				return sql_from(meta_, dialect_, std::move(stream_));
			}
			
			/**
			 * Select all columns in same order as defined.
			 */
			sql_from all();

			//
			//	aggregate functions
			//
			sql_from count();	//!< count(*)
			sql_from count(std::size_t);
			sql_from sum(std::size_t);
			sql_from avg(std::size_t);
			
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
			sql_insert(meta_table_ptr, dialect, std::stringstream&&);
			
			/*
			 * satisfy gcc < 5.5
			 */
#if __cplusplus < 201703L
			sql_insert(sql_insert&&);
#endif
			
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
			sql_update(meta_table_ptr, dialect, std::stringstream&&);
			
			template < typename EXPR >
			sql_where where(EXPR const& expr)
			{
				stream_ 
				<< "WHERE "
				;
				expr.serialize(stream_, meta_, dialect_, false);
				return sql_where(meta_, dialect_, std::move(stream_));
			}

			/**
			 * Constructs a WHERE clause for the key element(s) of the table.
			 */
			sql_where by_key();
		};

		/**
		 * DELETE ...
		 */
		class sql_remove : public base
		{
		public:
			sql_remove(meta_table_ptr, dialect, std::stringstream&&);
			
			/*
			 * satisfy gcc < 5.5
			 */
#if __cplusplus < 201703L
			sql_remove(sql_remove&&);
#endif
			
			template < typename EXPR >
			sql_where where(EXPR const& expr)
			{
				stream_
					<< "WHERE "
					;
				expr.serialize(stream_, meta_, dialect_, false);
				return sql_where(meta_, dialect_, std::move(stream_));
			}

			/**
			 * Combine all conditions with AND
			 */
			sql_where by_key();
		};


		/**
		 * Generate SQL commands for a single table.
		 */
		class command : public base
		{
		public:
			command(meta_table_ptr, dialect);
			
			/**
			 * Create a select clause.
			 */
			sql_select select();
			
			/**
			 * Constructs an SQL CREATE statement for this table.
			 */
			sql_create create();
			
			/**
			 * Constructs an SQL DELETE statement for this table.
			 */
			sql_remove remove();

			/**
			 * Constructs an SQL INSERT statement for this table.
			 */
			sql_insert insert();

			/**
			 * Constructs an SQL UPDATE statement for this table.
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
				
 				return sql_update(meta_, dialect_, std::move(stream_));
			}

			/**
			 * Constructs an SQL UPDATE for all data columns
			 * of the table.
			 */
			sql_update update();
			
		private:
			void clear();
			void clear(std::string const&);
					};
	}	
}

#endif	//	CYNG_SQL_H

