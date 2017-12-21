/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/store/record.h>
#include <cyng/factory/factory.hpp>

namespace cyng 
{
	namespace store 
	{
		record::record(meta_table_ptr meta)
		: meta_(meta)
		, key_()
		, data_()
		, generation_(0)
		{}

		record::record(meta_table_ptr meta, key_type const& key, data_type const& data, std::uint64_t generation)
		: meta_(meta)
		, key_(key)
		, data_(data)
		, generation_(generation)
		{}
		
		record::record(record const& rec)
		: meta_(rec.meta_)
		, key_(rec.key_)
		, data_(rec.data_)
		, generation_(rec.generation_)
		{}
		
		bool record::empty() const
		{
			return !meta_->check_record(key_, data_);
		}
		
		object record::operator[](std::size_t idx) const
		{
			return get(idx);
		}
		
		object record::operator[](std::string col) const
		{
			const auto r = meta_->get_record_index(col);
			return (r.second)
			? get(r.first)
			: make_object()
			;
		}
		

		object record::get(std::size_t idx) const
		{
			if (meta_->is_key(idx))
			{
				return key_.at(idx);
			}
			else if (meta_->is_body(idx))
			{
				//
				//	fix the offset
				//
				const auto r = meta_->get_body_index(idx);
				if (r.second)
				{
					return data_.at(r.first);
				}
			}
			return make_object();
		}
		
		std::uint64_t record::get_generation() const
		{
			return generation_;
		}

	}	//	store	
}






