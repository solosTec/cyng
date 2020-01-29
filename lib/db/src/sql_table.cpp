/*
* The MIT License (MIT)
*
* Copyright (c) 2018 Sylko Olzscher
*
*/


#include <cyng/db/sql_table.h>
#include <cyng/value_cast.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/numeric/conversion/cast.hpp>

namespace cyng
{
	attr_map_t to_attr_map(table::meta_table_ptr meta, db::result_ptr result)
	{
		BOOST_ASSERT_MSG(result, "no result available");
		attr_map_t amap;
		if (result)
		{
			meta->loop([meta, result, &amap](table::column&& col) {

				const auto r = result->get(static_cast<int>(col.pos_ + 1), col.type_, col.width_);
				if (!r)
				{
					std::cerr
						<< "couldn't read column "
						<< col.name_
						<< " of table "
						<< meta->get_name()
						<< std::endl
						;
				}
				amap.emplace(col.pos_, r);
			});

			BOOST_ASSERT_MSG(amap.size() == meta->size(), "result incomplete");
		}

		return amap;
	}

	param_map_t to_param_map(table::meta_table_ptr meta, db::result_ptr result)
	{
		BOOST_ASSERT_MSG(result, "no result available");
		param_map_t pmap;
		if (result)
		{
			meta->loop([meta, result, &pmap](table::column&& col) {

				const auto r = result->get(boost::numeric_cast<int>(col.pos_ + 1), col.type_, col.width_);
				if (!r)
				{
					std::cerr
						<< "couldn't read column "
						<< col.name_
						<< " of table "
						<< meta->get_name()
						<< std::endl
						;
				}
				pmap.emplace(col.name_, r);
			});

			BOOST_ASSERT_MSG(pmap.size() == meta->size(), "result incomplete");
		}

		return pmap;
	}

	tuple_t to_tuple(table::meta_table_ptr meta, db::result_ptr result)
	{
		BOOST_ASSERT_MSG(result, "no result available");
		tuple_t tpl;
		if (result)
		{
			meta->loop([meta, result, &tpl](table::column&& col) {

				const auto r = result->get(boost::numeric_cast<int>(col.pos_ + 1), col.type_, col.width_);
				if (!r)
				{
					std::cerr
						<< "couldn't read column "
						<< col.name_
						<< " of table "
						<< meta->get_name()
						<< std::endl
						;
				}
				tpl.push_back(r);
			});

			BOOST_ASSERT_MSG(tpl.size() == meta->size(), "result incomplete");
		}

		return tpl;
	}

	table::record to_record(table::meta_table_ptr meta, db::result_ptr result)
	{
		if (result)
		{
			table::key_type key;
			table::data_type data;
			std::uint64_t generation{ 0 };
			meta->loop([meta, result, &key, &data, &generation](cyng::table::column&& col) {

				auto const obj = result->get(boost::numeric_cast<int>(col.pos_ + 1), col.type_, col.width_);
				if (!obj)
				{
					std::cerr
						<< "couldn't read column "
						<< col.name_
						<< " of table "
						<< meta->get_name()
						<< std::endl
						;
				}
				else
				{

					if (col.pk_)
					{
						key.push_back(obj);
					}
					else
					{
						//
						//	automatic generation detection
						//
						if (boost::algorithm::equals(col.name_, "gen") && (col.type_ == TC_UINT64))
						{
							generation = cyng::value_cast<std::uint64_t>(obj, 0);
						}
						else
						{
							data.push_back(obj);
						}
					}
				}
			});

			return table::record(meta->derive_cache_table(), key, data, generation);
		}

		return table::make_empty_record(meta);
	}

}

