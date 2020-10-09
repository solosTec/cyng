/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/store/table.h>
#include <cyng/store/transform.hpp>
#include <iostream>

namespace cyng 
{
	namespace store 
	{
		table::table(table const& other)
		: publisher()
			, meta_(other.meta_)
			, data_()
			, index_()
		{}

		table::table(cyng::table::meta_table_ptr mtp)
		: publisher()
			, meta_(mtp)
 			, data_()
			, index_()
		{}
		
		table::table(table&& tbl)
		: publisher(std::move(tbl))
			, meta_(std::move(tbl.meta_))
 			, data_(std::move(tbl.data_))
			, index_(std::move(tbl.index_))
		{}
		
		table::~table()
		{
 			//disconnect();
		}
		
		std::size_t table::size() const
		{
			return data_.size();
		}

		void table::reserve(std::size_t count)
		{
			data_.reserve(count);
		}

		
		cyng::table::meta_table_interface const& table::meta() const
		{
			BOOST_ASSERT(meta_);
			return *meta_;
		}
		
		cyng::table::meta_table_ptr table::meta_ptr() const
		{
			return meta_;
		}

		void table::clear(boost::uuids::uuid source)
		{
			data_.clear();
			index_.clear();
 			this->publisher::clear_signal_(this, source);
		}
		
		bool table::insert(cyng::table::key_type const& key
			, cyng::table::data_type const& data
			, std::uint64_t generation
			, boost::uuids::uuid source)
		{
			//	prevent structural integrity
			if (meta_->check_record(key, data))
			{
				//	second is true if the pair was actually inserted.
 				if (data_.emplace(std::piecewise_construct
 				, std::forward_as_tuple(key)
 				, std::forward_as_tuple(make_object(data), generation)).second)	
				{
					auto const idx = meta_->get_index();
					if (idx.second) {
						index_.emplace(data.at(idx.first), key);
					}

					this->publisher::insert_signal_(this, key, data, generation, source);
					return true;
				}
			}
			return false;
		}
		
		bool table::merge(cyng::table::key_type const& key
			, cyng::table::data_type&& data
			, std::uint64_t generation
			, boost::uuids::uuid source)
		{
			//	secure data consistency.
			if (meta_->check_record(key, data))
			{
				//	second is true if the pair was actually inserted.
				auto r = data_.emplace(std::piecewise_construct
					, std::forward_as_tuple(key)
					, std::forward_as_tuple(make_object(data), generation));

				if (!r.second) {

					//
					//	key already exists - overwrite with new data
					//
					update(r.first, key, std::move(data), source);
				}
				else {

					auto const idx = meta_->get_index();
					if (idx.second) {
						index_.emplace(data.at(idx.first), key);
					}

					//
					//	new key inserted
					//
					this->publisher::insert_signal_(this, key, data, generation, source);
				}
				return true;
			}
			return false;
		}

		bool table::update(cyng::table::key_type const& key, cyng::table::data_type&& data, std::uint64_t generation, boost::uuids::uuid source)
		{
			//	prevent structural integrity
			if (meta_->check_record(key, data))
			{
				//
				//	search for key
				//
				auto r = find(key);
				if (r.second) {

					//
					//	key exists - overwrite with new data
					//
					update(r.first, key, std::move(data), source);
					return true;
				}
			}
			return false;
		}

		bool table::erase(cyng::table::key_type const& key, boost::uuids::uuid source)
		{
			auto const idx = meta_->get_index();
			if (idx.second) {

				//
				//	update index
				//
				auto r = find(key);
				if (r.second) {
					BOOST_ASSERT((*r.first).second.obj_.get_class().tag() == TC_VECTOR);
					cyng::table::data_type const* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
					BOOST_ASSERT(ptr != nullptr);
					if (ptr == nullptr)	return false;
					//auto pos = index_.find(ptr->at(idx.first));

					//
					//	remove from index (no check)
					//
					index_.erase(ptr->at(idx.first));

					//
					//	remove by iterator
					//
					data_.erase(r.first);
					this->publisher::remove_signal_(this, key, source);
					return true;
				}
			}
			else if (meta_->check_key(key))
			{

				//	second is true if the pair was actually inserted.
				if (data_.erase(key) != 0)
				{
					this->publisher::remove_signal_(this, key, source);
					return true;
				}
			}
			return false;
		}

		std::pair<table::table_type::const_iterator,bool> table::find(cyng::table::key_type const& key) const
		{
			if (meta_->check_key(key))
			{
				auto const pos = data_.find(key);
				return std::make_pair(pos, pos != data_.end());
			}
			return std::make_pair(data_.end(), false);
		}
		
		void table::update(table::table_type::const_iterator pos
			, cyng::table::key_type const& key
			, cyng::table::data_type&& data
			, boost::uuids::uuid source)
		{
			//
			//	key exists - overwrite with new data
			//
			unique_lock_t ul((*pos).second.m_);

			BOOST_ASSERT((*pos).second.obj_.get_class().tag() == TC_VECTOR);
			cyng::table::data_type const* ptr = object_cast<cyng::table::data_type>((*pos).second.obj_);
			BOOST_ASSERT(ptr != nullptr);
			if (ptr == nullptr)	return;

			//
			//	walk over all attributes and update/modify if required
			//
			BOOST_ASSERT(ptr->size() == data.size());
			for (std::size_t idx = 0; idx < ptr->size(); ++idx) {

				//
				//	compare each column
				//
				if (data.at(idx) != (*ptr).at(idx)) {

					//
					//	increase generation counter
					//
					++const_cast<std::uint64_t&>((*pos).second.generation_);

					//
					//	broadcast modification
					//
					this->publisher::modify_signal_(this, key, attr_t(idx, data.at(idx)), (*pos).second.generation_, source);

					//
					//	apply modification
					//
					swap(data.at(idx), (*const_cast<cyng::table::data_type*>(ptr)).at(idx));
				}
			}
		}

		bool table::exist(cyng::table::key_type const& key) const
		{
			return find(key).second;
		}
		
		cyng::table::record table::lookup(cyng::table::key_type const& key) const
		{
			std::pair<table::table_type::const_iterator, bool> r = find(key);
			if (r.second)
			{
				cyng::table::data_type const* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				return cyng::table::record(meta_, key, *ptr, (*r.first).second.generation_);
			}

			//	empty result
			return cyng::table::make_empty_record(meta_);
		}
		
		cyng::table::record table::lookup_by_index(object obj) const
		{
			if (meta_->has_index()) {
				auto const pos = index_.find(obj);
				if (pos != index_.end()) {
					return lookup(pos->second);
				}
			}

			//	empty result
			return cyng::table::make_empty_record(meta_);
		}

		object table::lookup(cyng::table::key_type const& key, std::size_t idx) const
		{
			std::pair<table::table_type::const_iterator, bool> r = find(key);
			if (r.second)
			{
				cyng::table::data_type const* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				if (idx < ptr->size())	return ptr->at(idx);
			}

			//	empty result
			return make_object();
		}

		object table::lookup(cyng::table::key_type const& key, std::string const& name) const
		{
			auto const r = meta_->get_body_index(name);
			return (r.second) 
				? lookup(key, r.first)
				: make_object()
				;
		}
		
		bool table::modify(cyng::table::key_type const& key, attr_t&& attr, boost::uuids::uuid source)
		{
			std::pair<table::table_type::const_iterator, bool> r = find(key);
			if (r.second)
			{
				BOOST_ASSERT((*r.first).second.obj_.get_class().tag() == TC_VECTOR);

				//
				//	Write lock record. Only one thread/writer can modify this record
				//	at the same time.
				//
				unique_lock_t ul((*r.first).second.m_);

				cyng::table::data_type const* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				BOOST_ASSERT(ptr != nullptr);
				BOOST_ASSERT(attr.first < (*ptr).size());

				if (ptr != nullptr)
				{
					//
					//	First distribute modification and than apply changes.
					//	Send modify signal only in case the values
					//	are really different.
					//
 					if (attr.second != (*ptr)[attr.first])
					{
						//
						//	increase generation counter
						//
						++const_cast<std::uint64_t&>((*r.first).second.generation_);
						
						//
						//	broadcast modification
						//
						this->publisher::modify_signal_(this, key, attr, (*r.first).second.generation_, source);
						
						//
						//	apply modification
						//
						swap(attr.second, (*const_cast<cyng::table::data_type*>(ptr))[attr.first]);
					}
					return true;
				}
			}
			return false;
		}
		
		bool table::modify(cyng::table::key_type const& key, param_t const& param, boost::uuids::uuid source)
		{
			const std::pair<std::size_t, bool> r = meta_->get_body_index(param.first);
			return (r.second)
			? modify(key, attr_t(r.first, param.second), source)
			: false
			;
		}
		
		std::size_t table::loop(std::function<bool(cyng::table::record const&)> f) const
		{
			std::size_t counter = this->size();
			for (auto const& row : data_)
			{
				auto dp = object_cast<cyng::table::data_type>(row.second.obj_);
				if (dp != nullptr)
				{
					//	shared lock
					shared_lock_t ul(row.second.m_);

					//
					//	false terminates the loop
					//
					if (!f(cyng::table::record(meta_, row.first, *object_cast<cyng::table::data_type>(row.second.obj_), row.second.generation_)))	break;
					--counter;
				}
			}
			return counter;
		}

		cyng::table::record table::find_first(attr_t&& attr) const
		{
			//	empty record
			cyng::table::record result = cyng::table::make_empty_record(meta_);


			//
			//	check range
			//
			if (meta_->size() < attr.first)	return result;

			//
			//	search first matching record
			//
			loop([&](cyng::table::record const& rec)->bool {

				if (rec[attr.first] == attr.second)
				{
					result = rec;
					return false;
				}
				return true;
			});
			return result;
		}

		cyng::table::record table::find_first(param_t&& param) const
		{
			//	empty record
			cyng::table::record result = cyng::table::make_empty_record(meta_);

			//
			//	check column name
			//
			auto const check = meta_->get_record_index(param.first);
			if (!check.second)	return result;	//	param has no valid column name

			//
			//	search first matching record
			//
			loop([&](cyng::table::record const& rec)->bool {

				if (rec[param.first] == param.second)
				{
					result = rec;
					return false;
				}
				return true;
			});
			return result;
		}

		cyng::table::key_list_t table::find_all(attr_t&& attr) const
		{
			//	empty list
			cyng::table::key_list_t result;

			//
			//	check range
			//
			if (meta_->size() < attr.first)	return result;

			//
			//	search first matching record
			//
			loop([&](cyng::table::record const& rec)->bool {

				if (rec[attr.first] == attr.second)
				{
					result.push_back(rec.key());
				}
				return true;
			});

			return result;
		}

		cyng::table::key_list_t table::find_all(param_t&& param) const
		{
			//	empty list
			cyng::table::key_list_t result;

			//
			//	check column name
			//
			auto const check = meta_->get_record_index(param.first);
			if (!check.second)	return result;	//	param has no valid column name

			//
			//	search first matching record
			//
			loop([&](cyng::table::record const& rec)->bool {

				if (rec[param.first] == param.second)
				{
					result.push_back(rec.key());
				}
				return true;
			});
			return result;
		}

		cyng::table::record table::min_record() const
		{
			auto pos = std::min_element(data_.begin(), data_.end(), [](table_type::value_type const& lhs, table_type::value_type const& rhs) {
				BOOST_ASSERT(lhs.first.size() == rhs.first.size());
				auto p1 = lhs.first.begin();
				auto p2 = rhs.first.begin();
				for (; p1 != lhs.first.end() && p2 != rhs.first.end(); ++p1, ++p2) {
					if (*p1 < *p2)	return true;
					if (*p2 < *p1)	return false;
				}
				return false;
			});

			if (pos != data_.end()) {
				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>(pos->second.obj_);
				return cyng::table::record(meta_, pos->first, *ptr, pos->second.generation_);
			}
			return cyng::table::make_empty_record(meta_);
		}

		cyng::table::record table::max_record() const
		{
			auto pos = std::max_element(data_.begin(), data_.end(), [](table_type::value_type const& lhs, table_type::value_type const& rhs) {
				BOOST_ASSERT(lhs.first.size() == rhs.first.size());
				auto p1 = lhs.first.begin();
				auto p2 = rhs.first.begin();
				for (; p1 != lhs.first.end() && p2 != rhs.first.end(); ++p1, ++p2) {
					if (*p1 < *p2)	return true;
					if (*p2 < *p1)	return false;
				}
				return false;
			});

			if (pos != data_.end()) {
				cyng::table::data_type const* ptr = object_cast<cyng::table::data_type>(pos->second.obj_);
				return cyng::table::record(meta_, pos->first, *ptr, pos->second.generation_);
			}
			return cyng::table::make_empty_record(meta_);
		}

		cyng::table::record table::nth_record(std::size_t idx) const
		{
			if (idx < size()) {
				auto pos = data_.begin();
				std::advance(pos, idx);
				cyng::table::data_type const* ptr = object_cast<cyng::table::data_type>(pos->second.obj_);
				return cyng::table::record(meta_, pos->first, *ptr, pos->second.generation_);
			}
			return cyng::table::make_empty_record(meta_);
		}

		vector_t table::convert(bool col_names) const
		{
			vector_t vec;

			if (col_names) {
				vec.reserve(size() + 1);

				//
				//	generate header entry
				//
				tuple_t header;
				meta_->loop([&](cyng::table::column&& col) {
					header.push_back(make_object(col.name_));
				});

				//
				//	insert header record
				//
				vec.push_back(make_object(header));
			}
			else {
				vec.reserve(size());
			}

			//
			//	append all records
			//
			loop([&](cyng::table::record const& rec)->bool {

				vec.push_back(make_object(rec.convert_data()));
				return true;	//	continue
			});

			return vec;
		}

	}	//	store
	
	std::size_t erase(store::table* tbl, table::key_list_t const& keys, boost::uuids::uuid source)
	{
		BOOST_ASSERT(tbl != nullptr);
		std::size_t counter{ 0 };
		std::for_each(keys.begin(), keys.end(), [&](table::key_type const& key) {
			if (tbl->erase(key, source))
			{
				++counter;
			}
		});
		return counter;
	}

	namespace traits
	{
	
#if !defined(__CPP_SUPPORT_N2235)
		const char type_tag<cyng::store::table>::name[] = "table";
#endif
	}	// traits	
}

namespace std
{
	size_t hash<cyng::store::table>::operator()(cyng::store::table const& t) const noexcept
	{
		return std::hash<std::string>{}(t.meta().get_name());
	}

	bool equal_to<cyng::store::table>::operator()(cyng::store::table const& t1, cyng::store::table const& t2) const noexcept
	{
		return t1.meta().get_name() == t2.meta().get_name();
	}

	bool less<cyng::store::table>::operator()(cyng::store::table const& t1, cyng::store::table const& t2) const noexcept
	{
		return t1.meta().get_name() < t2.meta().get_name();
	}

}






