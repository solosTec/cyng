/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/table/record.h>
#include <cyng/factory/factory.hpp>
#include <boost/algorithm/string.hpp>

namespace cyng 
{
	namespace table 
	{
		record::record(meta_table_ptr meta)
		: meta_(meta)
		, key_(meta_->size() - meta_->body_size())
		, data_(meta_->body_size())
		, generation_(0)
		{}

		record::record(meta_table_ptr meta, key_type const& key, data_type const& data, std::uint64_t generation)
		: meta_(meta)
		, key_(key)
		, data_(data)
		, generation_(generation)
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

		record& record::operator=(data_type const& data)
		{
			if (meta_->check_body(data)) {
				data_ = data;
			}
			else {
				data_.resize(meta_->body_size());
			}
			return *this;
		}

		record& record::read_data(record const& rec)
		{
			rec.meta_->loop_body([&](column&& col) {
				this->set(col.name_, rec[col.name_]);
			});
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
		
		void record::set(std::size_t idx, object obj)
		{
			if (meta_->is_key(idx))
			{
				BOOST_ASSERT_MSG(idx < key_.size(), "key index out of range");
				if (idx < key_.size()) key_.at(idx) = obj;
			}
			else if (meta_->is_body(idx))
			{
				//
				//	fix the offset
				//
				const auto r = meta_->get_body_index(idx);
				BOOST_ASSERT_MSG(r.first < data_.size(), "body index out of range");
				if (r.first < data_.size())	data_.at(r.first) = obj;
			}
		}

		/**
		 * write access by name
		 */
		bool record::set(std::string col, object obj)
		{
			const auto r = meta_->get_record_index(col);
			if (r.second) {
				set(r.first, obj);
				return true;
			}
			return false;
		}

		object record::get(std::size_t idx) const
		{
			if (meta_->is_key(idx))
			{
				BOOST_ASSERT_MSG(idx < key_.size(), "key index out of range");
				if (idx < key_.size()) return key_.at(idx);
			}
			else if (meta_->is_body(idx))
			{
				//
				//	fix the offset
				//
				const auto r = meta_->get_body_index(idx);
				if (r.second)
				{
					BOOST_ASSERT_MSG(r.first < data_.size(), "body index out of range");
					if (r.first < data_.size())	return data_.at(r.first);
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
			//	generation_ is part of the data body
			//
			data["gen"] = make_object(generation_);

			//
			//	additional parameters
			//
			std::for_each(pm.begin(), pm.end(), [&data](param_map_value const& val) {
				data[val.first] = val.second;
			});

			return cyng::tuple_factory(cyng::param_factory("key", key)
				, cyng::param_factory("data", data));
		}

		tuple_t record::convert_data() const
		{
			tuple_t tpl;

			meta_->loop([&](column&& col) {
				tpl.push_back(get(col.pos_));
			});

			return tpl;
		}

		data_type record::shrink_data(std::initializer_list<std::string> il) const
		{
			std::vector<std::string> all_names, dif_names;

			//
			//	get all column names of the data body
			//
			meta_->loop_body([&](column&& col) {
				all_names.push_back(col.name_);
				});

			//
			//	get the difference
			//
			std::set_difference(all_names.begin(), all_names.end(), il.begin(), il.end(),
				std::inserter(dif_names, dif_names.begin()));

			//
			//	collect reduced data body
			//
			data_type data;
			for (auto const& name : dif_names) {
				data.push_back((*this)[name]);
			}
			return data;
		}


		policy to_policy(std::string str)
		{
			if (boost::algorithm::equals(str, "merge"))	return POLICY_MERGE;
			else if (boost::algorithm::starts_with(str, "subst"))	return POLICY_SUBSTITUTE;
			return POLICY_APPEND;
		}

	}	//	table	
}






