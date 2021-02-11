/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_SQL_HPP
#define CYNG_SQL_HPP


#include <cyng/sql/dialect.h>
#include <cyng/store/meta.h>
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

		using clause_t = std::vector<std::string>;

		namespace details {
			class base
			{
			public:
				base(dialect, clause_t&&);
				std::string to_str() const;
				std::string operator()() const;

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
			public:
				sql_where(dialect, clause_t&&);
				void where(std::string);
			};

			class sql_from : public base
			{
			public:
				sql_from(dialect, clause_t&&);
				sql_where from(std::string);
				sql_where from(meta_sql const&);
			private:
				clause_t reset_clause();

			};
		}


		/**
		 * A select returns a FROM clause
		 */
		class select
		{
		public:
			select(dialect);

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
		 * Insert table statement
		 */
		class insert : public details::base
		{
		public:
			insert(dialect, meta_sql const&);

		private:
			void columns(meta_sql const&);
		};

	}
}

#endif	

