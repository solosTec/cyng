#include <cyng/sql/dsl/placeholder.h>

#include <sstream>

namespace cyng {
    namespace sql {
        placeholder::placeholder(std::string name)
            : name_(name) {}

        std::string placeholder::to_string(dialect d, meta_sql const &ms) const {
            std::stringstream ss;
            //	detect special datetime types to use with "julianday()"
            auto const idx = ms.get_index_by_name(name_);
            auto const &col = ms.get_column(idx);
            return (d == dialect::SQLITE && (col.type_ == TC_TIME_POINT || col.type_ == TC_DATE)) ? "julianday(?)" : "?";
        }

        placeholder make_placeholder(std::string name) { return placeholder(name); }

    } // namespace sql
} // namespace cyng
