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
#include <cyng/parse/mac.h>
#include <cyng/parse/version.h>
#include <cyng/parse/duration.h>
#include <cyng/parse/buffer.h>
#include <cyng/log/conv.h>

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

	object restore(std::string const& val, std::uint32_t code)
	{
		switch (code) {
		case TC_BOOL:
			//	true is encoded as "true"
			return make_object(boost::algorithm::equals("true", val));
			//case cyng::TC_CHAR:
		case cyng::TC_FLOAT:
			return make_object(std::stof(val));
		case cyng::TC_DOUBLE:
			return make_object(std::stod(val));
		case cyng::TC_FLOAT80:
			return make_object(std::stold(val));
		case cyng::TC_UINT8:
			return make_object(static_cast<std::uint8_t>(std::stoul(val, nullptr, 16)));
		case cyng::TC_UINT16:
			return make_object(static_cast<std::uint16_t>(std::stoul(val, nullptr, 16)));
		case cyng::TC_UINT32:
			return make_object(static_cast<std::uint32_t>(std::stoul(val, nullptr, 16)));
		case cyng::TC_UINT64:
			return make_object(static_cast<std::uint64_t>(std::stoull(val, nullptr, 16)));
		case cyng::TC_INT8:
			return make_object(static_cast<std::int8_t>(std::stoi(val)));
		case cyng::TC_INT16:
			return make_object(static_cast<std::int16_t>(std::stoi(val)));
		case cyng::TC_INT32:
			return make_object(static_cast<std::int32_t>(std::stoi(val)));
		case cyng::TC_INT64:
			return make_object(static_cast<std::int64_t>(std::stoi(val)));
			//case cyng::TC_STRING:	//	default
			//case cyng::TC_TIME_POINT: 
		case cyng::TC_MICRO_SECOND:		return make_object(to_microseconds(val));
		case cyng::TC_MILLI_SECOND:		return make_object(to_milliseconds(val));
		case cyng::TC_SECOND:			return make_object(to_seconds(val));
		case cyng::TC_MINUTE:			return make_object(to_minutes(val));
		case cyng::TC_HOUR:				return make_object(to_hours(val));

		case cyng::TC_VERSION:			return make_object(to_version(val));
		case cyng::TC_REVISION:			return make_object(to_revision(val));

		//case cyng::TC_CODE:
		//case cyng::TC_LABEL:
		case cyng::TC_SEVERITY:			return make_object(to_severity(val));
		case cyng::TC_BUFFER:			return make_object(to_buffer(val));
		case cyng::TC_MAC48:			return make_object(to_mac48(val));
		case cyng::TC_MAC64:			return make_object(to_mac64(val));
		case cyng::TC_IP_TCP_ENDPOINT:
		{
			//auto const r = parse_tcp_ep(val);
			//return (r.second)
			//	? make_object(r.first)
			//	: make_object(val)
			//	;
		}
		break;

		case cyng::TC_IP_ADDRESS:
			return make_object(boost::asio::ip::make_address(val));

		default:
			break;
		}

		return make_object(val);
	}

}

