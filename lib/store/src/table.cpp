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
		{}

		table::table(cyng::table::meta_table_ptr mtp)
		: publisher()
		, meta_(mtp)
 		, data_()
		{}
		
		table::table(table&& tbl)
		: publisher(std::move(tbl))
		, meta_(std::move(tbl.meta_))
 		, data_(std::move(tbl.data_))
		{}
		
		table::~table()
		{
 			//disconnect();
		}
		
		std::size_t table::size() const
		{
			return data_.size();
		}
		
		cyng::table::meta_table_interface const& table::meta() const
		{
			return *meta_;
		}
		
		void table::clear()
		{
			data_.clear();
 			this->publisher::clear_signal_(this);
		}
		
		bool table::insert(cyng::table::key_type const& key, cyng::table::data_type const& data, std::uint64_t generation)
		{
			//	prevent structural integrity
			if (meta_->check_record(key, data))
			{
				//	second is true if the pair was actually inserted.
 				if (data_.emplace(std::piecewise_construct
 				, std::forward_as_tuple(key)
 				, std::forward_as_tuple(make_object(data), generation)).second)	
				{
					this->publisher::insert_signal_(this, key, data, generation);
					return true;
				}
			}
			return false;
		}
		
		bool table::erase(cyng::table::key_type const& key)
		{
			//	prevent structural integrity
			if (meta_->check_key(key))
			{
				//	second is true if the pair was actually inserted.
				if (data_.erase(key) != 0)
				{
					this->publisher::remove_signal_(this, key);
					return true;
				}
			}
			return false;
		}
		
		std::pair<table::table_type::const_iterator,bool> table::find(cyng::table::key_type const& key) const
		{
			if (meta_->check_key(key))
			{
				auto pos = data_.find(key);
				return std::make_pair(pos, pos != data_.end());
			}
			return std::make_pair(data_.end(), false);
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
				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
				return cyng::table::record(meta_, key, *ptr, (*r.first).second.generation_);
			}

			//	empty result
			return cyng::table::record(meta_);
		}
		
		
		bool table::modify(cyng::table::key_type const& key, attr_t&& attr)
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

				const cyng::table::data_type* ptr = object_cast<cyng::table::data_type>((*r.first).second.obj_);
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
						this->publisher::modify_signal_(this, key, attr);
						
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
		
		bool table::modify(cyng::table::key_type const& key, param_t&& param)
		{
			const std::pair<std::size_t, bool> r = meta_->get_body_index(param.first);
			return (r.second)
			? modify(key, attr_t(r.first, param.second))
			: false
			;
		}
		
		std::size_t table::loop(std::function<void(cyng::table::record const&)> f) const
		{
			std::size_t counter = this->size();
			for (auto const& row : data_)
			{
				auto dp = object_cast<cyng::table::data_type>(row.second.obj_);
				if (dp != nullptr)
				{
					//	shared lock
					shared_lock_t ul(row.second.m_);
					f(cyng::table::record(meta_, row.first, *object_cast<cyng::table::data_type>(row.second.obj_), row.second.generation_));
					--counter;
				}
			}
			return counter;
		}

	}	//	store
	
	namespace traits
	{
	
#if defined(CYNG_LEGACY_MODE_ON)
		const char type_tag<cyng::store::table>::name[] = "table";
#endif
	}	// traits	
}







