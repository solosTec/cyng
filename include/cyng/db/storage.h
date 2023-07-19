/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */

#ifndef CYNG_DB_STORAGE_H
#define CYNG_DB_STORAGE_H

#include <cyng/db/details/result_interface.h>
#include <cyng/db/session.h>
#include <cyng/store/record.h>

namespace cyng {
    namespace db {
        class storage {
          public:
            using loop_f = std::function<bool(record &&)>;

          public:
            storage(session);

            /**
             * loop over all records in the specified table
             */
            std::size_t loop(meta_sql const &ms, loop_f);

          private:
            session s_;
        };
    } // namespace db

    /**
     * Read a result set into a key and and a body
     * object tuple.
     * The "gen" column from the SQL result will be stripped and
     * the meta data of the record will be adapted.
     *
     * @note The meta data of the table and the record are different.
     */
    record to_record(meta_sql const &ms, db::result_ptr);

    /**
     * Read a result set into an attribute map.
     */
    attr_map_t to_attr_map(meta_sql const &ms, db::result_ptr);

    /**
     * Read a result set into a parameter map.
     */
    param_map_t to_param_map(meta_sql const &ms, db::result_ptr);

    /**
     * Read a result set into an object tuple. All elements
     * are simple objects.
     */
    tuple_t to_tuple(meta_sql const &ms, db::result_ptr);

    /**
     * restore a data type from a string and a type code
     * Note: could throw
     */
    object restore(std::string const &, std::uint16_t);

} // namespace cyng

#endif
