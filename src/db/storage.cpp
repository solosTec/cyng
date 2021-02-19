/*
* The MIT License (MIT)
*
* Copyright (c) 2021 Sylko Olzscher
*
*/

#include <cyng/db/storage.h>
#include <cyng/db/details/statement_interface.h>
#include <cyng/sql/sql.hpp>
#include <cyng/obj/numeric_cast.hpp>

#ifdef _DEBUG_DB
#include <iostream>
#endif

#include <boost/algorithm/string.hpp>

namespace cyng
{
	namespace db 
	{
		storage::storage(session s)
			: s_(s)
		{}

		void storage::loop(meta_sql const& ms, loop_f f) {

			cyng::sql::select s(s_.get_dialect());
			auto const sql = s.all(ms, true).from(ms.get_name())();
#ifdef _DEBUG_DB
			std::cout << sql << std::endl;
#endif
			auto stmt = s_.create_statement();
			stmt->prepare(sql);

			//
			//	read all results
			//
			while (auto res = stmt->get_result()) {

				//
				//	Convert SQL result to record
				//	false terminates the loop
				//
				if (!f(to_record(ms, res)))	break;

			}
		}
	}

	record to_record(meta_sql const& ms, db::result_ptr result) {
		auto const m = to_mem(ms);

		if (result)
		{
			key_t key;
			data_t data;
			std::uint64_t generation{ 0 };

			int index{ 1 };
			ms.loop([&](std::size_t ridx, column_sql const& col, bool pk) {

				auto obj = result->get(index, col.type_, col.width_);
				BOOST_ASSERT(obj);
				if (obj) {
					if (pk) {
						key.push_back(obj);
					}
					else {
						//
						//	automatic generation detection
						//
						if (boost::algorithm::equals(col.name_, "gen") && (col.type_ == TC_UINT64)) {
							generation = cyng::numeric_cast<std::uint64_t>(obj, 0);
						}
						else {
							data.push_back(obj);
						}
					}
				}
				++index;
			});

			return record(m, key, data, generation);
		}
		return record(m);
	}

	attr_map_t to_attr_map(meta_sql const& ms, db::result_ptr result)
	{
		BOOST_ASSERT_MSG(result, "no result available");

		auto const m = to_mem(ms);
		attr_map_t amap;
		if (result)
		{
			int index{ 1 };
			ms.loop([&](std::size_t ridx, column_sql const& col, bool pk) {

				auto obj = result->get(index, col.type_, col.width_);
				BOOST_ASSERT(obj);
				if (obj) {
					amap.emplace(index, obj);
				}
				++index;

			});

			BOOST_ASSERT_MSG(amap.size() == ms.size(), "result incomplete");
		}

		return amap;
	}

	param_map_t to_param_map(meta_sql const& ms, db::result_ptr result)
	{
		BOOST_ASSERT_MSG(result, "no result available");

		auto const m = to_mem(ms);
		param_map_t pmap;
		if (result)
		{
			int index{ 1 };
			ms.loop([&](std::size_t ridx, column_sql const& col, bool pk) {

				auto obj = result->get(index, col.type_, col.width_);
				BOOST_ASSERT(obj);
				if (obj) {
					pmap.emplace(col.name_, obj);
				}
				++index;

			});

			BOOST_ASSERT_MSG(pmap.size() == ms.size(), "result incomplete");
		}

		return pmap;
	}

	tuple_t to_tuple(meta_sql const& ms, db::result_ptr result) {

		auto const m = to_mem(ms);
		tuple_t tpl;
		if (result)
		{
			int index{ 1 };
			ms.loop([&](std::size_t ridx, column_sql const& col, bool pk) {

				auto obj = result->get(index, col.type_, col.width_);
				BOOST_ASSERT(obj);
				if (obj) {
					tpl.push_back(obj);
				}
				++index;

				});

			BOOST_ASSERT_MSG(tpl.size() == ms.size(), "result incomplete");
		}

		return tpl;
	}

}

