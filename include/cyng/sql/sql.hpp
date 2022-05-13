/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_HPP
#define CYNG_SQL_HPP

#include <cyng/sql/dialect.h>
#include <cyng/sql/convert.hpp>
#include <cyng/store/meta.h>
#include <cyng/io/ostream.h>

#include <vector>
#include <boost/predef.h>

namespace cyng 
{
	namespace sql {

		//
		//	sql commands 
		//
		class select;
		class create;
		class insert;
		class update;
		class remove;
		class drop;

		/**
		 * expands to ?
		 */
		//struct placeholder	{};

		/**
		 * expands to all columns that are primary key
		 */
		struct pk {};

		/**
		 * expands to all columns that are not primary key
		 */
		struct body {};

		using clause_t = std::vector<std::string>;

		namespace details {
			class base
			{
			public:
				base(dialect, clause_t&&);
				std::string to_str() const;
				std::string operator()() const;

			protected:
				bool do_skip(std::string) const;

			private:
				static bool is_between_brackets(std::string const&, std::string const&);

			protected:
				dialect const dialect_;
				clause_t clause_;
			};

			//
			//	SELECT <columns>
			//	FROM < table>
			//	WHERE < predicate on rows>	2.
			//	GROUP BY < columns>	3.
			//	HAVING < predicate on groups>	4.
			//	ORDER BY < columns>	6.
			//	OFFSET	7.
			//	FETCH FIRST	8.
			//

			class sql_order_by : public base
			{
			public:
				sql_order_by(dialect, clause_t&&);
				//void offset(std::string);
			};

			class sql_having : public base
			{
			public:
				sql_having(dialect, clause_t&&);
				//void order_by(std::string);
				//void offset(std::string);
			};

			class sql_group_by : public base
			{
			public:
				sql_group_by(dialect, clause_t&&);
				//void having(std::string);
				//void order_by(std::string);
				//void offset(std::string);
			};

			class sql_where : public base
			{
				//friend details::sql_group_by;
			public:
				sql_where(dialect, meta_sql const& ms, clause_t&&);

				/**
				 * Use an arbitrary string as where clause.
				 */
				template < std::size_t N >
				sql_order_by where(const char(&c)[N]) {
					return order_by(std::string(c, N - 1));
				}
				
				sql_group_by where(std::string cond);
				sql_order_by order_by(std::string cond);
				sql_group_by where(pk);

				template<typename EXPR>
				details::sql_group_by where(EXPR expr) {
					return where(expr.to_str(dialect_, meta_));
				}

			private:
				/**
				 * Specify the record by it's primary key
				 */
				sql_group_by where(meta_sql const&, pk);
				clause_t reset_clause();

			private:
				meta_sql const& meta_;
			};

			class sql_from : public base
			{
			public:
				sql_from(dialect, meta_sql const& ms, clause_t&&);
				sql_where from(std::string);
				sql_where from();
			private:
				/**
				 * Use another table scheme
				 */
				sql_where from(meta_sql const&);
				clause_t reset_clause();
			private:
				meta_sql const& meta_;

			};

			class sql_values : public base
			{
			public:
				sql_values(dialect, clause_t&&);
			};

			/**
			 * substitute data type with placeholder
			 */
			std::string substitute_ph(dialect, type_code);

			/**
			 * handle datetime (for SQLite)
			 */
			std::string substitute_dt(std::string name, dialect, type_code);
		}


		/**
		 * A select returns a FROM clause
		 */
		class select
		{
		public:
			select(dialect, meta_sql const& ms);

			/**
			 * SELECT *
			 */
			details::sql_from all();
			details::sql_from all(meta_sql const&, bool qualified);

			/**
			 * SELECT COUNT(*)
			 */
			details::sql_from count();

			/**
			 * SELECT SUM(col)
			 */
			details::sql_from sum(std::string);
			details::sql_from sum(meta_sql const&, std::size_t);

			/**
			 * SELECT AVG(col)
			 */
			details::sql_from avg(std::string);
			details::sql_from avg(meta_sql const&, std::size_t);

		private:
			clause_t reset_clause();

		private:
			dialect const dialect_;
			clause_t clause_;
			meta_sql const& meta_;
		};

		/**
		 * Create table statement
		 */
		class create : public details::base
		{
		public:
			create(dialect, meta_sql const&);

		private:
			void columns(meta_sql const&);
			void pks(meta_sql const&);
		};

		/**
		 * Drop table statement
		 */
		class drop : public details::base
		{
		public:
			drop(dialect, meta_sql const&);
		};

		/**
		 * Insert table statement
		 */
		class insert : public details::base
		{
		public:
			insert(dialect, meta_sql const&);

			template <typename Head, typename ...Args >
			details::sql_values values(meta_sql const& m, Head&& v, Args &&... args) {

				clause_.push_back("VALUES");
				clause_.push_back("(");

				//
				//	insert values
				//
				BOOST_ASSERT(m.size() == sizeof...(Args) + 1);

				//
				//	ToDo: recursive insertion of values
				//
				clause_.push_back(to_sql(dialect_, v));
				((clause_.push_back(","),  clause_.push_back(to_sql(dialect_, args))), ...);

				clause_.push_back(")");
				return details::sql_values(dialect_, reset_clause(m));
			}

			/**
			 * @param n number of placeholders
			 */
			details::sql_values bind_values(meta_sql const& m);

		private:
			void columns(meta_sql const&);
			clause_t reset_clause(meta_sql const& m);

		};

		/**
		 * Update table statement
		 * UPDATE tbl SET col = val, ... WHERE cond
		 */
		class update : public details::base
		{
		public:
			update(dialect, meta_sql const&);

			/**
			 * update the complete table body
			 */
			details::sql_where set_placeholder();

			details::sql_where set_placeholder(std::size_t col);

		private:
			clause_t reset_clause(meta_sql const& m);

			/**
			 * update a single column and the "gen" column
			 *
			 * @param col zero based index of data body
			 */
			details::sql_where set_placeholder(meta_sql const& m, std::size_t col);

			/**
			 * update the complete table body
			 */
			details::sql_where set_placeholder(meta_sql const& m);

		private:
			meta_sql const& meta_;

		};

		/**
		 * Delete table statement
		 * DELETE FROM tbl WHERE cond
		 */
		class remove : public details::base
		{
		public:
			remove(dialect, meta_sql const&);

			/**
			 * @param clause any valid SQL clause after WHERE
			 */
			details::sql_group_by where(std::string clause);

			/**
			 * All primary keys in there WHERE clause chained by logical AND.
			 */
			details::sql_group_by where(pk);

			/**
			 * Accept an expression template
			 */
			template<typename EXPR>
			details::sql_group_by where(EXPR expr) {
				return self().where(expr.to_str(dialect_, meta_));
			}

		private:
			details::sql_group_by where(meta_sql const& m, pk);
			details::sql_where self();
			clause_t reset_clause();

		private:
			meta_sql const& meta_;

		};

	}
}

#endif	

