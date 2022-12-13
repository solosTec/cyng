/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#include <boost/algorithm/string.hpp>
#include <cyng/sql/sql.hpp>
#include <string>

namespace cyng {
    namespace sql {

        //
        //	+-- SELECT -----------------------------------------------+
        //
        select::select(dialect d, meta_sql const &ms)
            : dialect_(d)
            , clause_({"SELECT"})
            , meta_(ms) {}

        clause_t select::reset_clause() {
            clause_t tmp = std::move(clause_);
            clause_.push_back("SELECT");
            return tmp;
        }

        details::sql_from select::all() {
            clause_.push_back("*");
            return details::sql_from(dialect_, meta_, std::move(clause_));
        }
        details::sql_from select::all(meta_sql const &m, bool qualified) {

            bool init = false;
            m.loop([&](std::size_t idx, column_sql const &col, bool pk) -> void {
                if (!init) {
                    init = true;
                } else {
                    clause_.push_back(",");
                }

                //
                //	Build a full qualified name if requested and
                //	check for datetime() in case of SQLite3.
                //	An optionally "ROWID" is included.
                //
                if (qualified) {
                    clause_.push_back(details::substitute_dt(m.get_name() + "." + col.name_, dialect_, col.type_));
                } else {
                    clause_.push_back(details::substitute_dt(col.name_, dialect_, col.type_));
                }
            });
            return details::sql_from(dialect_, meta_, reset_clause());
        }

        details::sql_from select::count() {
            clause_.push_back("COUNT(*)");
            return details::sql_from(dialect_, meta_, reset_clause());
        }

        details::sql_from select::sum(std::string col) {
            clause_.push_back("SUM(" + col + ")");
            return details::sql_from(dialect_, meta_, reset_clause());
        }

        details::sql_from select::sum(meta_sql const &meta, std::size_t idx) { return sum(meta.get_name(idx)); }

        details::sql_from select::avg(std::string col) {
            clause_.push_back("AVG(" + col + ")");
            return details::sql_from(dialect_, meta_, reset_clause());
        }

        details::sql_from select::avg(meta_sql const &meta, std::size_t idx) { return avg(meta.get_name(idx)); }

        //
        //	+-- CREATE -----------------------------------------------+
        //
        create::create(dialect d, meta_sql const &m)
            : details::base(d, clause_t{"CREATE", "TABLE"}) {
            if (has_feature(dialect_, IF_NOT_EXISTS)) {
                clause_.push_back("IF NOT EXISTS");
            }

            clause_.push_back(m.get_name());

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

        void create::columns(meta_sql const &m) {

            bool init = false;
            m.loop([&](std::size_t idx, column_sql const &col, bool pk) -> void {
                //
                //	ROWID is already in place.
                //
                if (!do_skip(col.name_)) {
                    if (!init) {
                        init = true;
                    } else {
                        clause_.push_back(",");
                    }

                    clause_.push_back(col.name_);
                    clause_.push_back(get_field_type(dialect_, col.type_, col.width_));
                }
            });
        }

        void create::pks(meta_sql const &m) {
            clause_.push_back(",");
            clause_.push_back("PRIMARY KEY");
            clause_.push_back("(");
            bool init = false;
            m.loop([&](std::size_t idx, column_sql const &col, bool pk) -> void {
                if (pk) {
                    if (!init) {
                        init = true;
                    } else {
                        clause_.push_back(",");
                    }
                    clause_.push_back(col.name_);
                }
            });

            clause_.push_back(")");
        }

        //
        //	+-- DROP -------------------------------------------------+
        //
        drop::drop(dialect d, meta_sql const &m)
            : details::base(d, clause_t{"DROP", "TABLE"}) {
            if (has_feature(dialect_, IF_NOT_EXISTS)) {
                clause_.push_back("IF EXISTS");
            }

            clause_.push_back(m.get_name());
        }

        //
        //	+-- INSERT -----------------------------------------------+
        //
        insert::insert(dialect d, meta_sql const &m)
            : details::base(d, clause_t{"INSERT", "INTO", m.get_name()}) {
            columns(m);
        }

        void insert::columns(meta_sql const &m) {

            clause_.push_back("(");

            bool init = false;
            m.loop([&](std::size_t idx, column_sql const &col, bool pk) -> void {
                //
                //	ROWID will set automatically
                //
                if (!do_skip(col.name_)) {
                    if (!init) {
                        init = true;
                    } else {
                        clause_.push_back(",");
                    }

                    clause_.push_back(col.name_);
                }
            });
            clause_.push_back(")");
        }

        clause_t insert::reset_clause(meta_sql const &m) {
            clause_t tmp = std::move(clause_);
            clause_.push_back("INSERT");
            clause_.push_back("INTO");
            clause_.push_back(m.get_name());
            columns(m);
            return tmp;
        }

        details::sql_values insert::bind_values(meta_sql const &m) {
            clause_.push_back("VALUES");
            clause_.push_back("(");

            //
            //	insert placeholder
            //
            bool init = false;
            m.loop([&](std::size_t idx, column_sql const &col, bool pk) -> void {
                //
                //	ROWID will set automatically
                //
                if (!do_skip(col.name_)) {
                    if (!init) {
                        init = true;
                    } else {
                        clause_.push_back(",");
                    }
                    clause_.push_back(details::substitute_ph(dialect_, col.type_));
                }
            });

            clause_.push_back(")");
            return details::sql_values(dialect_, reset_clause(m));
        }

        //
        //	+-- UPDATE -----------------------------------------------+
        //
        update::update(dialect d, meta_sql const &m)
            : details::base(d, clause_t{"UPDATE", m.get_name()})
            , meta_(m) {}

        details::sql_where update::set_placeholder() { return set_placeholder(meta_); }

        details::sql_where update::set_placeholder(std::size_t col) { return set_placeholder(meta_, col); }

        details::sql_where update::set_placeholder(meta_sql const &ms) {

            clause_.push_back("SET");

            bool init = false;
            ms.loop([&](std::size_t idx, column_sql const &col, bool pk) -> void {
                if (!pk) {
                    if (!init) {
                        init = true;
                    } else {
                        clause_.push_back(",");
                    }
                    clause_.push_back(col.name_);
                    clause_.push_back("=");
                    clause_.push_back(details::substitute_ph(dialect_, col.type_));
                }
            });

            return details::sql_where(dialect_, ms, reset_clause(ms));
        }

        details::sql_where update::set_placeholder(meta_sql const &ms, std::size_t idx) {

            clause_.push_back("SET");
            BOOST_ASSERT(idx < ms.body_size());

            auto const &col = ms.get_body_column(idx + 1); //	skip "gen"

            //
            //	value
            //
            clause_.push_back(col.name_);
            clause_.push_back("=");
            clause_.push_back(details::substitute_ph(dialect_, col.type_));

            //
            //	gen (first column after the key)
            //
            auto const &gen = ms.get_column(ms.key_size());
            BOOST_ASSERT(boost::algorithm::equals(gen.name_, "gen"));
            clause_.push_back(",");
            clause_.push_back(gen.name_);
            clause_.push_back("=");
            clause_.push_back(details::substitute_ph(dialect_, gen.type_));

            return details::sql_where(dialect_, ms, reset_clause(ms));
        }

        clause_t update::reset_clause(meta_sql const &m) {
            clause_t tmp = std::move(clause_);
            clause_.push_back("UPDATE");
            clause_.push_back(m.get_name());
            return tmp;
        }

        //
        //	+-- REMOVE -----------------------------------------------+
        //
        remove::remove(dialect d, meta_sql const &m)
            : details::base(d, clause_t{"DELETE", "FROM", m.get_name()})
            , meta_(m) {}

        details::sql_where remove::self() { return details::sql_where(dialect_, meta_, reset_clause()); }

        details::sql_group_by remove::where(std::string clause) { return self().where(clause); }

        details::sql_group_by remove::where(meta_sql const &m, pk type) { return self().where(type); }

        details::sql_group_by remove::where(pk) { return this->where(meta_, pk{}); }

        clause_t remove::reset_clause() {
            //	remains unchanged
            return clause_;
        }

        namespace details {

            //
            //	+-- base ---------------------------------------------+
            //
            base::base(dialect d, clause_t &&vec)
                : dialect_(d)
                , clause_(std::move(vec)) {}

            std::string base::to_string() const {
                std::string s;
                bool init = false;
                for (const auto &e : clause_) {
                    if (!init) {
                        init = true;
                    } else {
                        //	no space before an "," or ")"
                        if (!boost::algorithm::equals(e, ",") && !boost::algorithm::equals(e, "(")) {

                            //	no space between brackets
                            if (!is_between_brackets(s, e))
                                s += ' ';
                        }
                    }
                    s += e;
                }
                return s;
            }

            bool base::is_between_brackets(std::string const &s, std::string const &e) {
                return (boost::algorithm::equals(e, ")") && !s.empty() && s.back() == ')');
            }

            std::string base::operator()() const { return to_string(); }

            bool base::do_skip(std::string name) const {
                return (dialect::SQLITE == dialect_) && boost::algorithm::equals(name, "ROWID");
            }

            //
            //	+-- from ---------------------------------------------+
            //
            sql_from::sql_from(dialect d, meta_sql const &ms, clause_t &&vec)
                : base(d, std::move(vec))
                , meta_(ms) {
                clause_.push_back("FROM");
            }
            clause_t sql_from::reset_clause() {
                clause_t tmp = std::move(clause_);
                clause_.push_back("FROM");
                return tmp;
            }

            sql_where sql_from::from(std::string name) {
                clause_.push_back(name);
                return sql_where(dialect_, meta_, reset_clause());
            }
            sql_where sql_from::from() { return from(meta_); }
            sql_where sql_from::from(meta_sql const &m) { return from(m.get_name()); }

            //
            //	+-- group by -----------------------------------------+
            //
            sql_group_by::sql_group_by(dialect d, clause_t &&vec)
                : base(d, std::move(vec)) {}

            //
            //	+-- having -------------------------------------------+
            //
            sql_having::sql_having(dialect d, clause_t &&vec)
                : base(d, std::move(vec)) {}

            //
            //	+-- order by -----------------------------------------+
            //
            sql_order_by::sql_order_by(dialect d, clause_t &&vec)
                : base(d, std::move(vec)) {}

            //
            //	+-- where --------------------------------------------+
            //
            sql_where::sql_where(dialect d, meta_sql const &ms, clause_t &&vec)
                : base(d, std::move(vec))
                , meta_(ms) {}

            sql_group_by sql_where::where(std::string cond) {
                clause_.push_back("WHERE");
                clause_.push_back(cond);
                return sql_group_by(dialect_, reset_clause());
            }

            sql_group_by sql_where::where(meta_sql const &m, pk) {
                clause_.push_back("WHERE");
                bool init = false;
                m.loop([&](std::size_t idx, column_sql const &col, bool pk) -> void {
                    if (pk) {
                        if (!init) {
                            init = true;
                        } else {
                            clause_.push_back("AND");
                        }
                        clause_.push_back(col.name_);
                        clause_.push_back("=");
                        clause_.push_back(details::substitute_ph(dialect_, col.type_));
                    }
                });
                return sql_group_by(dialect_, reset_clause());
            }

            sql_order_by sql_where::order_by(std::string cond) {

                clause_.push_back("ORDER");
                clause_.push_back("BY");
                clause_.push_back(cond);
                return sql_order_by(dialect_, reset_clause());
            }

            sql_group_by sql_where::where(pk type) { return where(meta_, type); }

            clause_t sql_where::reset_clause() { return std::move(clause_); }

            //
            //	+-- values -------------------------------------------+
            //
            sql_values::sql_values(dialect d, clause_t &&vec)
                : base(d, std::move(vec)) {}

            //
            //	+-- substitute ---------------------------------------+
            //
            std::string substitute_ph(dialect d, type_code code) {
                switch (d) {
                case dialect::SQLITE:
                    switch (code) {
                    case TC_TIME_POINT:
                    case TC_DATE: return "julianday(?)";
                    default: break;
                    }
                    break;
                default: break;
                }

                return "?";
            }

            std::string substitute_dt(std::string name, dialect d, type_code code) {
                switch (d) {
                case dialect::SQLITE:
                    switch (code) {
                    case TC_TIME_POINT: return "datetime(" + name + ")";
                    default: break;
                    }
                    break;
                default: break;
                }

                return name;
            }

        } // namespace details
    }     // namespace sql
} // namespace cyng
