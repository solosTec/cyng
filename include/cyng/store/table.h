/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2021 Sylko Olzscher
 *
 */
#ifndef CYNG_STORE_TABLE_H
#define CYNG_STORE_TABLE_H

#include <cyng/obj/compare.hpp>
#include <cyng/obj/function_cast.hpp>
#include <cyng/store/meta.h>
#include <cyng/store/pub.h>
#include <cyng/store/record.h>

#include <memory>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

namespace cyng {

    class store;
    class table : public pub, public std::enable_shared_from_this<table> {

        //
        //	wants to access the mutex
        //
        friend store;

        using table_t = std::unordered_map<key_t, body, std::hash<key_t>, std::equal_to<key_t>>;

      public:
        table() = delete;

        /**
         * create a in-memory table from meta info
         */
        table(meta_store const &);

        virtual ~table();

        /**
         * @return number of elements in the table
         */
        std::size_t size() const noexcept;

        /**
         * Sets the number of buckets to the number needed to accomodate at least count elements
         * without exceeding maximum load factor and rehashes the container.
         */
        void reserve(std::size_t);

        /**
         * @return meta data
         */
        meta_store const &meta() const noexcept;

        /**
         * Clears the table contents.
         */
        void clear(boost::uuids::uuid source);

        /**
         * Place a new record into the table. Failes if a records
         * with the same key already exists.
         *
         * @param key the record key
         * @param data the body to insert
         * @param generation only needed for insert operations
         * @param source identifier for data source
         * @return true if the record was actually inserted.
         */
        bool insert(key_t const &key, data_t const &data, std::uint64_t generation, boost::uuids::uuid source);

        /**
         * Place a new record into the table. If a records
         * with the same key already exists it will be replaced.
         *
         * @param key the record key
         * @param data the body to insert
         * @param generation only needed for insert operations
         * @param source identifier for data source
         * @return true if the record was actually inserted or modified
         */
        bool merge(key_t const &key, data_t &&data, std::uint64_t generation, boost::uuids::uuid source);

        /**
         * If a matching record was found, the record will be write/exclusive locked.
         * The modification signal is send only in case the new and old values are different.
         *
         * @param key the record key
         * @param attr a specific attribute of the record body.
         * @return true if new and old values were different
         */
        bool modify(key_t const &key, attr_t &&attr, boost::uuids::uuid source);
        bool modify(key_t const &key, attr_map_t &&am, boost::uuids::uuid source);

        /**
         * If a matching record was found, the record will be write/exclusive locked.
         * The modification signal is send only in case the new and old values are different.
         *
         * @param key the record key
         * @param param a specific parameter of the record body.
         * @return true if new and old values were different
         */
        bool modify(key_t const &key, param_t const &param, boost::uuids::uuid source);

        bool modify(key_t const &key, param_map_t const &pm, boost::uuids::uuid source);

        /** @brief search for primary key
         *
         * Complexity O(1) up to O(N)
         *
         * @brief simple record lookup
         * @return true if record was found
         */
        bool exists(key_t const &key) const;

        /**
         * Complexity O(1) up to O(N)
         *
         * @brief simple record lookup
         */
        record lookup(key_t const &key) const;

        /**
         * Lookup by index.
         *
         * @param key search key
         * @param idx number of index
         */
        record lookup(key_t const &key, std::size_t idx) const;

        /**
         * @param key the record key
         * @return true if the record was actually deleted
         */
        bool erase(key_t const &key, boost::uuids::uuid source);

        /**
         * If predicate returns true, the element will be deleted.
         *
         * @param pred predicate for selecting records to be deleted
         * @return number of erased elements
         */
        std::size_t erase_if(std::function<bool(record &&)> pred, boost::uuids::uuid source);

        /**
         * If pred returns true, the record will be removed.
         */
        template <typename... Args> void erase_if(std::function<bool(Args...)> pred, boost::uuids::uuid source) {

            using F = std::function<bool(Args...)>;
            for (auto pos = data_.begin(); pos != data_.end();) {

                //
                //	read lock this record
                //
                std::shared_lock<std::shared_mutex> ulock(pos->second.m_);

                auto const rec = record(meta(), pos->first, pos->second.data_, pos->second.generation_);

                //
                //	unzip data and dispatch to specified function
                //
                auto const tpl = rec.get_tuple();
                if (function_call<F>(pred, tpl)) {

                    //
                    //	publish
                    //
                    forward(this, pos->first, pos->second.data_, source);

                    //
                    //	erase
                    //
                    pos = data_.erase(pos);
                } else {
                    ++pos;
                }
            }
        }

        /**
         * @return true if table is of type auto
         */
        virtual bool is_auto() const;

        /**
         * Find a record where the specified data column has the value v.
         */
        template <typename T> record find_first(std::size_t col, T const &v) {

            comparator cmp(v);
            auto pos = std::find_if(std::begin(data_), std::end(data_), [col, &cmp](table_t::value_type const &b) {
                return cmp(b.second.data_.at(col));
            });

            return (pos != std::end(data_)) ? record{meta(), pos->first, pos->second.data_, pos->second.generation_}
                                            : record{meta()};
        }

        /**
         * Find a record where the specified data column has the value v.
         */
        template <typename T> record find_first(std::string col, T const &v) {
            auto const idx = meta().get_body_index_by_name(col);
            return (idx != std::numeric_limits<std::size_t>::max()) ? find_first(idx, v) : record{meta()};
        }

        /** @brief Loop over all table entries.
         *
         * Construct for each data row a record and fires a callback.
         * If callback function return false the loop is exited.
         *
         * @return count of invalid/skipped records
         */
        std::size_t loop(std::function<bool(record &&, std::size_t)> f) const;

        /**
         * Loop over a selected records.
         *
         * This is the same mechanism used by the task library. A tuple of objects will be automatically
         * unpacked and dispatched to the specified function object.
         */
        template <typename... Args> void loop(std::function<bool(Args...)> cb) const {

            using F = std::function<bool(Args...)>;
            for (auto const &row : data_) {
                //
                //	read lock this record
                //
                std::shared_lock<std::shared_mutex> ulock(row.second.m_);

                auto const rec = record(meta(), row.first, row.second.data_, row.second.generation_);
                //
                //	unzip data and dispatch to specified function
                //
                auto const tpl = rec.get_tuple();
                if (!function_call<F>(cb, tpl))
                    break;
            }
        }

        /**
         * Convert the table into a vector. Usefull for
         * serialization to CSV.
         *
         * @param col_names if true the first data tuple contains the
         * column names
         */
        vector_t to_vector(bool col_names) const;

        /**
         * update a value by a function.
         */
        template <typename T> bool compute(key_t const &key, std::size_t idx, std::function<T(T)> calc, boost::uuids::uuid source) {

            auto pos = data_.find(key);
            if (pos != data_.end()) {

                //
                //	write lock this record
                //
                std::unique_lock<std::shared_mutex> ulock(pos->second.m_);

                auto attr = make_attr(idx, calc(value_cast<T>(pos->second.data_.at(idx), T{})));

                if (pos->second.data_.at(idx) != attr.second) {

                    //
                    //	publish
                    //
                    this->pub::forward(this, key, attr, pos->second.data_, pos->second.generation_, source);

                    //
                    //	apply
                    //
                    swap(pos->second.data_.at(attr.first), attr.second);

                    //
                    //	update generation
                    //
                    ++pos->second.generation_;

                    return true;
                }
            }
            return false;
        }

        /**
         * update a value by a function.
         */
        template <typename T>
        bool compute(key_t const &key, std::string name, std::function<T(T)> calc, boost::uuids::uuid source) {
            auto const idx = meta().get_body_index_by_name(name);
            return compute(key, idx, calc, source);
        }

      private:
        /**
         * @return true  if insertion happened, otherwise false
         */
        bool emplace(key_t const &key, data_t const &data, std::uint64_t generation);

        void update(table_t::iterator, key_t const &key, data_t &&data, boost::uuids::uuid source);

        void charge(slot_ptr) override;

      private:
        meta_store const meta_;
        table_t data_;

        /**
         * A sync object for this table
         */
        mutable std::shared_mutex m_;
    };

    using table_ptr = std::shared_ptr<table>;
} // namespace cyng
#endif
