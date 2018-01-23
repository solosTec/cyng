/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/store/db.h>
#include <cyng/table/meta.hpp>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/object_cast.hpp>
#include <cyng/value_cast.hpp>

#include <chrono>

namespace cyng 
{
	namespace store 
	{

		db::db()
		: tables_(cyng::table::make_meta_table<1, 3>("db"
			, { "name", "table", "created", "state" }
			, { TC_STRING, traits::PREDEF_TABLE, TC_TIME_POINT, TC_UINT32 }
			, { 64, 0, 0, 0 }))
		, m_()
		{}
		
		db::~db()
		{
			tables_.clear();
		}
		
		bool db::create_table(cyng::table::meta_table_ptr ptr)
		{
			//	start with generation 1 and default state 0
			unique_lock_t ul(this->m_);
			return tables_.insert(cyng::table::key_generator(ptr->get_name())
				, cyng::table::data_generator(table(ptr), std::chrono::system_clock::now(), static_cast<std::uint32_t>(0)), 1);
		}
		
		std::size_t db::size() const
		{
			shared_lock_t ul(this->m_);
			return tables_.size();
		}	

		void db::clear(std::string const& name)
		{
			shared_lock_t ul(this->m_);
			access([](table* tbl)->void {
				tbl->clear();
			}, write_access(name));
		}

		bool db::insert(std::string const& name, cyng::table::key_type const& key, cyng::table::data_type const& data, std::uint64_t generation)
		{
			shared_lock_t ul(this->m_);
#if defined(CYNG_STD_APPLY_OFF)
			bool b = false;
			access([&key, &data, generation, &b](table* tbl)->void {

				b = tbl->insert(key, data, generation);

			}, write_access(name));
			return b;
#else
			return access([&key, &data, generation](table* tbl)->bool {
				return tbl->insert(key, data, generation);
			}, write_access(name));
#endif
		}

		bool db::erase(std::string const& name, cyng::table::key_type const& key)
		{
			shared_lock_t ul(this->m_);
#if defined(CYNG_STD_APPLY_OFF)
			bool b = false;
			access([&key, &b](table* tbl)->void {

				b = tbl->erase(key);

			}, write_access(name));
			return b;
#else
			return access([&key](table* tbl)->bool {
				return tbl->erase(key);
			}, write_access(name));
#endif
		}

		bool db::modify(std::string const& name, cyng::table::key_type const& key, attr_t&& attr)
		{
			shared_lock_t ul(this->m_);
#if defined(CYNG_STD_APPLY_OFF)
			bool b = false;
			access([&key, &attr, &b](table* tbl)->void {

				b = tbl->modify(key, std::move(attr));

			}, write_access(name));
			return b;
#else
			return access([&key, &attr](table* tbl)->bool {
				return tbl->modify(key, std::move(attr));
			}, write_access(name));
#endif
		}

		bool db::modify(std::string const& name, cyng::table::key_type const& key, param_t&& param)
		{
			shared_lock_t ul(this->m_);
#if defined(CYNG_STD_APPLY_OFF)
			bool b = false;
			access([&key, &param, &b](table* tbl)->void {

				b = tbl->modify(key, std::move(param));

			}, write_access(name));
			return b;
#else
			return access([&key, &param](table* tbl)->bool {
				return tbl->modify(key, std::move(param));
			}, write_access(name));
#endif
		}

		cyng::table::meta_table_ptr db::meta(std::string const & name) const
		{
			shared_lock_t ul(this->m_);
			auto r = tables_.find(cyng::table::key_generator(name));
			if (r.second)
			{
				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				BOOST_ASSERT(ptr != nullptr);
				if (ptr != nullptr)	return object_cast<table>(ptr->at(0))->meta_;
			}
			return cyng::table::meta_table_ptr();
		}

		connections_t db::get_listener(std::string const& name
			, publisher::insert_signal::slot_type const& isig
			, publisher::remove_signal::slot_type const& rsig
			, publisher::clear_signal::slot_type const& csig
			, publisher::modify_signal::slot_type const& msig)
		{
			shared_lock_t ul(this->m_);
			auto r = tables_.find(cyng::table::key_generator(name));
			if (r.second)
			{
				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				BOOST_ASSERT(ptr != nullptr);
				if (ptr != nullptr)	const_cast<table*>(object_cast<table>(ptr->at(0)))->get_listener(isig, rsig,  csig, msig);
			}
			return connections_t();
		}

		std::size_t db::size(std::string const& name) const
		{
			shared_lock_t ul(this->m_);
			auto r = tables_.find(cyng::table::key_generator(name));
			if (r.second)
			{
				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				BOOST_ASSERT(ptr != nullptr);
				if (ptr != nullptr)	return object_cast<table>(ptr->at(0))->size();
			}
			return 0;
		}

		bool db::set_state(std::string const& name, std::uint32_t state)
		{
			unique_lock_t ul(this->m_);
			auto r = tables_.find(cyng::table::key_generator(name));
			if (r.second)
			{
				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				BOOST_ASSERT(ptr != nullptr);
				if (ptr != nullptr)
				{
					auto v = const_cast<std::uint32_t*>(object_cast<std::uint32_t>(ptr->at(2)));
					if (v != nullptr)
					{
						*v = state;
						return true;
					}
				}
			}
			return false;
		}

		std::uint32_t db::get_state(std::string const& name)
		{
			shared_lock_t ul(this->m_);
			auto r = tables_.find(cyng::table::key_generator(name));
			if (r.second)
			{
				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				BOOST_ASSERT(ptr != nullptr);
				if (ptr != nullptr)	return value_cast<std::uint32_t>(ptr->at(2), 0);
			}
			return 0;
		}

	}	//	store
	
	namespace traits
	{
	
#if defined(CYNG_LEGACY_MODE_ON)
		const char type_tag<cyng::store::db>::name[] = "db";
#endif
	}	// traits		
}








