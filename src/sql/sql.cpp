/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2021 Sylko Olzscher
 *
 */ 

#include <cyng/sql/sql.hpp>
//#include <algorithm>
//#include <iterator>
#include <string>
#include <boost/algorithm/string.hpp>

namespace cyng
{
	namespace sql {

		select::select(dialect d)
			: dialect_(d)
			, clause_({ "SELECT" })
		{}

		clause_t select::reset_clause() {
			clause_t tmp = std::move(clause_);
			clause_.push_back("SELECT");
			return tmp;
		}


		details::sql_from select::all() {
			clause_.push_back("*");
			return details::sql_from(dialect_, std::move(clause_));
		}
		details::sql_from select::all(meta_sql const& m, bool qualified) {

			m.loop([&](std::size_t idx, column_sql const& col, bool pk)->void {

				//
				//	ToDo: check for datetime() in case of SQLite3
				//
				if (qualified) {
					clause_.push_back(m.get_name() + "." + col.name_);
				}
				else {
					clause_.push_back(col.name_);
				}
				});
			return details::sql_from(dialect_, reset_clause());
		}


		details::sql_from select::count()	{
			clause_.push_back("COUNT(*)");
			return details::sql_from(dialect_, reset_clause());
		}

		details::sql_from select::sum(std::string col) {
			clause_.push_back("SUM(" + col + ")");
			return details::sql_from(dialect_, reset_clause());
		}

		details::sql_from select::sum(meta_sql const& meta, std::size_t idx) {
			return sum(meta.get_name(idx));
		}

		details::sql_from select::avg(std::string col) {
			clause_.push_back("AVG(" + col + ")");
			return details::sql_from(dialect_, reset_clause());
		}

		details::sql_from select::avg(meta_sql const& meta, std::size_t idx) {
			return avg(meta.get_name(idx));
		}

		//
		//	CREATE
		//
		create::create(dialect d, meta_sql const& m)
			: details::base(d, clause_t{ "CREATE", "TABLE"} )
		{
			clause_.push_back(m.get_name());

			if (has_feature(dialect_, IF_NOT_EXISTS))	{
				clause_.push_back("IF NOT EXISTS");
			}

			clause_.push_back("(");

			//
			//	create column definitions
			//	column_1 data_type PRIMARY KEY,
			//	column_2 data_type NOT NULL,
			//
			columns(m);

			//
			//	write primary key constraint
			//
			if (m.has_pk()) {
				pks(m);
			}

			//
			//	ToDo: write (named) unique constraints
			//

			clause_.push_back(")");
		}

		void create::columns(meta_sql const& m) {

			bool init = false;
			m.loop([&](std::size_t idx, column_sql const& col, bool pk)->void {
				if (!init)	{
					init = true;
				}
				else	{
					clause_.push_back(",");
				}

				clause_.push_back(col.name_);
				clause_.push_back(get_field_type(dialect_, col.type_, col.width_));

				});
		}

		void create::pks(meta_sql const& m) {
			clause_.push_back(",");
			clause_.push_back("PRIMARY KEY");
			clause_.push_back("(");
			bool init = false;
			m.loop([&](std::size_t idx, column_sql const& col, bool pk)->void {
				if (pk) {
					if (!init) {
						init = true;
					}
					else {
						clause_.push_back(",");
					}
					clause_.push_back(col.name_);
				}

				});

			clause_.push_back(")");
		}

		//
		//	INSERT
		//
		insert::insert(dialect d, meta_sql const& m)
			: details::base(d, clause_t{ "INSERT", "INTO" })
		{
			clause_.push_back(m.get_name());
			columns(m);
		}

		void insert::columns(meta_sql const& m) {

			clause_.push_back("(");

			bool init = false;
			m.loop([&](std::size_t idx, column_sql const& col, bool pk)->void {
				if (!init) {
					init = true;
				}
				else {
					clause_.push_back(",");
				}

				clause_.push_back(col.name_);

				});
			clause_.push_back(")");
		}

		namespace details {
			base::base(dialect d, clause_t&& vec)
				: dialect_(d)
				, clause_(std::move(vec))
			{}

			std::string base::to_str() const {
				std::string s;
				bool init = false;
				for (const auto& e : clause_) {
					if (!init) {
						init = true;
					}
					else {
						//	no space before an "," or ")" 
						if (!boost::algorithm::equals(e, ",")
							&& !boost::algorithm::equals(e, "(")) {

							//	no space between brackets
							if (!is_between_brackets(s, e))	s += ' ';
							
						}
					}
					s += e;
				}
				return s;
			}

			bool base::is_between_brackets(std::string const& s, std::string const& e) {
				return (boost::algorithm::equals(e, ")")
					&& !s.empty()
					&& s.back() == ')');
			}

			std::string base::operator()() const {
				return to_str();
			}

			sql_from::sql_from(dialect d, clause_t&& vec)
				: base(d, std::move(vec))
			{
				clause_.push_back("FROM");
			}
			clause_t sql_from::reset_clause() {
				clause_t tmp = std::move(clause_);
				clause_.push_back("FROM");
				return tmp;
			}

			sql_where sql_from::from(std::string name) {
				clause_.push_back(name);
				return sql_where(dialect_, reset_clause());
			}
			sql_where sql_from::from(meta_sql const& m) {
				return from(m.get_name());
			}

			sql_where::sql_where(dialect d, clause_t&& vec)
				: base(d, std::move(vec))
			{}

			void sql_where::where(std::string cond) {
				clause_.push_back("WHERE");
				clause_.push_back(cond);
			}

			sql_group_by::sql_group_by(dialect d, clause_t&& vec)
				: base(d, std::move(vec))
			{}

			sql_having::sql_having(dialect d, clause_t&& vec)
				: base(d, std::move(vec))
			{}

			sql_order_by::sql_order_by(dialect d, clause_t&& vec)
				: base(d, std::move(vec))
			{}

		}
	}
}

