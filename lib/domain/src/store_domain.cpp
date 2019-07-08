/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include <cyng/vm/domain/store_domain.h>
#include <cyng/store/db.h>
#include <cyng/vm/context.h>
#include <cyng/vm/controller.h>
#include <cyng/vm/generator.h>
#include <cyng/tuple_cast.hpp>
#include <algorithm>
#ifdef _DEBUG
#include <boost/algorithm/string.hpp>
#endif

namespace cyng 
{
	void db_req_insert(store::db& db, cyng::context& ctx)
	{
		//
		//	* table name
		//	* record key
		//	* record data
		//	* generation
		//	
		auto const frame = ctx.get_frame();
		auto tpl = cyng::tuple_cast<
			std::string,			//	[0] table name
			vector_t,				//	[1] key
			vector_t,				//	[2] data
			std::uint64_t			//	[3] generation
		>(frame);

		ctx.queue(generate_invoke("log.msg.debug", "db.req.insert", std::get<0>(tpl), std::get<1>(tpl).size(), std::get<2>(tpl).size()));

		//
		//	key
		//
		std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

		//
		//	data
		//
		std::reverse(std::get<2>(tpl).begin(), std::get<2>(tpl).end());

		//
		//	insert
		//
		if (!db.insert(std::get<0>(tpl), std::get<1>(tpl), std::get<2>(tpl), std::get<3>(tpl), ctx.tag()))
		{
			ctx.queue(generate_invoke("log.msg.warning"
				, "db.req.insert - failed"
				, std::get<0>(tpl)
				, std::get<1>(tpl)
				, std::get<3>(tpl)));
		}
	}

	void db_req_merge(store::db& db, cyng::context& ctx)
	{
		auto const frame = ctx.get_frame();
		auto tpl = cyng::tuple_cast<
			std::string,			//	[0] table name
			vector_t,				//	[1] key
			vector_t,				//	[2] data
			std::uint64_t			//	[3] generation
		>(frame);

		ctx.queue(generate_invoke("log.msg.debug", "db.req.merge", std::get<0>(tpl), std::get<1>(tpl).size(), std::get<2>(tpl).size()));

		//
		//	key
		//
		std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

		//
		//	data
		//
		std::reverse(std::get<2>(tpl).begin(), std::get<2>(tpl).end());

		//
		//	merge
		//	* name table name
		//	* key the record key
		//	* data the body to insert
		//	* generation only needed for insert operations
		//	* source identifier for data source
		//	* return true if the record was actually inserted or modified
		//
		if (!db.merge(std::get<0>(tpl), std::get<1>(tpl), std::move(std::get<2>(tpl)), std::get<3>(tpl), ctx.tag()))
		{
			ctx.queue(generate_invoke("log.msg.warning"
				, "db.req.merge - failed"
				, std::get<0>(tpl)
				, std::get<1>(tpl)
				, std::get<3>(tpl)));
		}
	}

	void db_req_update(store::db& db, cyng::context& ctx)
	{
		auto const frame = ctx.get_frame();
		auto tpl = cyng::tuple_cast<
			std::string,			//	[0] table name
			vector_t,				//	[1] key
			vector_t,				//	[2] data
			std::uint64_t			//	[3] generation
		>(frame);

		ctx.queue(generate_invoke("log.msg.debug", "db.req.update", std::get<0>(tpl), std::get<1>(tpl).size(), std::get<2>(tpl).size()));

		//
		//	key
		//
		std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

		//
		//	data
		//
		std::reverse(std::get<2>(tpl).begin(), std::get<2>(tpl).end());

		//
		//	update
		//	* name table name
		//	* key the record key
		//	* data the body to insert
		//	* generation only needed for insert operations
		//	* source identifier for data source
		//	* return true if the record was actually inserted or modified
		//
		if (!db.update(std::get<0>(tpl), std::get<1>(tpl), std::move(std::get<2>(tpl)), std::get<3>(tpl), ctx.tag()))
		{
			ctx.queue(generate_invoke("log.msg.warning"
				, "db.req.update - failed"
				, std::get<0>(tpl)
				, std::get<1>(tpl)
				, std::get<3>(tpl)));
		}
	}

	void db_req_modify_by_attr(store::db& db, context& ctx)
	{
		auto const frame = ctx.get_frame();

		ctx.queue(generate_invoke("log.msg.debug", "db.req.modify.by.attr", frame));

		auto tpl = cyng::tuple_cast<
			std::string,			//	[0] table name
			vector_t,				//	[1] key
			attr_t					//	[2] data
		>(frame);

		//
		//	key
		//
		std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

		if (!db.modify(std::get<0>(tpl), std::get<1>(tpl), std::move(std::get<2>(tpl)), ctx.tag()))
		{
			ctx.queue(generate_invoke("log.msg.warning", "db.req.modify.by.attr - failed", std::get<0>(tpl), std::get<1>(tpl)));
		}
	}

	void db_req_modify_by_param(store::db& db, context& ctx)
	{
		auto const frame = ctx.get_frame();

		ctx.queue(generate_invoke("log.msg.debug", "db.req.modify.by.param", frame));

		auto tpl = cyng::tuple_cast<
			std::string,		//	[0] table name
			vector_t,			//	[1] key
			param_t				//	[2] data
		>(frame);

		//
		//	key
		//
		std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

		if (!db.modify(std::get<0>(tpl), std::get<1>(tpl), std::move(std::get<2>(tpl)), ctx.tag()))
		{
			ctx.queue(generate_invoke("log.msg.warning", "db.req.modify.by.param - failed", std::get<0>(tpl), std::get<1>(tpl)));
		}
	}

	void db_req_remove(store::db& db, context& ctx)
	{
		auto const frame = ctx.get_frame();

		ctx.queue(generate_invoke("log.msg.debug", "db.req.remove", frame));

		auto tpl = cyng::tuple_cast<
			std::string,			//	[0] table name
			vector_t				//	[1] key
		>(frame);

		//
		//	key
		//
		std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

		if (!db.erase(std::get<0>(tpl), std::get<1>(tpl), ctx.tag()))
		{
			ctx.queue(generate_invoke("log.msg.warning", "db.req.remove - failed", std::get<0>(tpl), std::get<1>(tpl)));
		}
	}

	void db_clear(store::db& db, context& ctx)
	{
		auto const frame = ctx.get_frame();

		auto const tpl = cyng::tuple_cast<
			std::string			//	[0] table name
		>(frame);

		ctx.queue(generate_invoke("log.msg.debug", "db.clear", std::get<0>(tpl)));

		db.clear(std::get<0>(tpl), ctx.tag());
	}

	void register_store(store::db& db, context& ctx)
	{
		ctx.queue(register_function("db.req.insert", 4, std::bind(&db_req_insert, std::ref(db), std::placeholders::_1)));
		ctx.queue(register_function("db.req.merge", 4, std::bind(&db_req_merge, std::ref(db), std::placeholders::_1)));
		ctx.queue(register_function("db.req.update", 4, std::bind(&db_req_update, std::ref(db), std::placeholders::_1)));
		ctx.queue(register_function("db.req.modify.by.attr", 3, std::bind(&db_req_modify_by_attr, std::ref(db), std::placeholders::_1)));
		ctx.queue(register_function("db.req.modify.by.param", 3, std::bind(&db_req_modify_by_param, std::ref(db), std::placeholders::_1)));
		ctx.queue(register_function("db.req.remove", 2, std::bind(&db_req_remove, std::ref(db), std::placeholders::_1)));
		ctx.queue(register_function("db.clear", 1, std::bind(&db_clear, std::ref(db), std::placeholders::_1)));
	}

	void register_store(store::db& db, controller& vm)
	{
		vm.register_function("db.req.insert", 4, std::bind(&db_req_insert, std::ref(db), std::placeholders::_1));
		vm.register_function("db.req.merge", 4, std::bind(&db_req_merge, std::ref(db), std::placeholders::_1));
		vm.register_function("db.req.update", 4, std::bind(&db_req_update, std::ref(db), std::placeholders::_1));
		vm.register_function("db.req.modify.by.attr", 3, std::bind(&db_req_modify_by_attr, std::ref(db), std::placeholders::_1));
		vm.register_function("db.req.modify.by.param", 3, std::bind(&db_req_modify_by_param, std::ref(db), std::placeholders::_1));
		vm.register_function("db.req.remove", 2, std::bind(&db_req_remove, std::ref(db), std::placeholders::_1));
		vm.register_function("db.clear", 1, std::bind(&db_clear, std::ref(db), std::placeholders::_1));
	}

}
