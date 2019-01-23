/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/table/record.h>
#include <cyng/factory/factory.hpp>

namespace cyng 
{
	namespace table 
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

		record& record::operator=(record const& other)
		{
			if (this != &other)
			{
				meta_ = other.meta_;
				key_ = other.key_;
				data_ = other.data_;
				generation_ = other.generation_;

			}
			return *this;
		}

		
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
		
		key_type const&	record::key() const
		{
			return key_;
		}

		data_type const& record::data() const
		{
			return data_;
		}

		std::uint64_t record::get_generation() const
		{
			return generation_;
		}

		tuple_t record::convert() const
		{
			param_map_t key, data;
			meta_->loop([&](column&& col){
				if (col.pk_) {
					key[col.name_] = get(col.pos_);
				}
				else {
					data[col.name_] = get(col.pos_);
				}
			});

			//
			//	generation_ is part of the data body
			//
			data["gen"] = make_object(generation_);

			return cyng::tuple_factory(cyng::param_factory("key", key)
				, cyng::param_factory("data", data));
		}

		tuple_t record::convert(param_map_t const& pm) const
		{
			param_map_t key, data;
			meta_->loop([&](column&& col) {
				if (col.pk_) {
					key[col.name_] = get(col.pos_);
				}
				else {
					data[col.name_] = get(col.pos_);
				}
			});

			//
			//	additional parameters
			//
			std::for_each(pm.begin(), pm.end(), [&data](param_map_value const& val) {
				data[val.first] = val.second;
			});

			return cyng::tuple_factory(cyng::param_factory("key", key)
				, cyng::param_factory("data", data)
				, cyng::param_factory("gen", generation_));
		}

		tuple_t record::convert_data() const
		{
			tuple_t tpl;

			meta_->loop([&](column&& col) {
				tpl.push_back(get(col.pos_));
			});

			return tpl;
		}

	}	//	table	
}






