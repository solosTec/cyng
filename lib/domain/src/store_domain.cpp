/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 


#include <cyng/vm/domain/store_domain.h>
#include <cyng/store/db.h>
#include <cyng/vm/context.h>
#include <cyng/vm/generator.h>
#include <cyng/tuple_cast.hpp>
#include <algorithm>
#ifdef _DEBUG
#include <boost/algorithm/string.hpp>
#endif

namespace cyng 
{

	void register_store(store::db& db, context& ctx)
	{
		ctx.attach(register_function("db.req.insert", 4, [&db](context& ctx) {

			const vector_t frame = ctx.get_frame();
			//ctx.attach(generate_invoke("log.msg.debug", "db.req.insert", frame));

			auto tpl = cyng::tuple_cast<
				std::string,			//	[0] table name
				vector_t,				//	[1] key
				vector_t,				//	[2] data
				std::uint64_t			//	[3] generation
			>(frame);

			ctx.attach(generate_invoke("log.msg.debug", "db.req.insert", std::get<0>(tpl), std::get<1>(tpl).size(), std::get<2>(tpl).size()));

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
				ctx.attach(generate_invoke("log.msg.warning"
					, "db.req.insert - failed"
					, std::get<0>(tpl)
					, std::get<1>(tpl)
					, std::get<3>(tpl)));
			}
		}));

		ctx.attach(register_function("db.req.modify.by.attr", 3, [&db](context& ctx) {
			const vector_t frame = ctx.get_frame();

			ctx.attach(generate_invoke("log.msg.debug", "db.req.modify.by.attr", frame));

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
				ctx.attach(generate_invoke("log.msg.warning", "db.req.modify.by.attr - failed", std::get<0>(tpl), std::get<1>(tpl)));
			}
		}));

		ctx.attach(register_function("db.req.modify.by.param", 3, [&db](context& ctx) {
			const vector_t frame = ctx.get_frame();

			ctx.attach(generate_invoke("log.msg.debug", "db.req.modify.by.param", frame));

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
				ctx.attach(generate_invoke("log.msg.warning", "db.req.modify.by.param - failed", std::get<0>(tpl), std::get<1>(tpl)));
			}
		}));

		ctx.attach(register_function("db.req.remove", 2, [&db](context& ctx) {
			const vector_t frame = ctx.get_frame();

			ctx.attach(generate_invoke("log.msg.debug", "db.req.remove", frame));

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
				ctx.attach(generate_invoke("log.msg.warning", "db.req.remove - failed", std::get<0>(tpl), std::get<1>(tpl)));
			}

		}));

		ctx.attach(register_function("db.clear", 1, [&db](context& ctx) {
			const vector_t frame = ctx.get_frame();
			//ctx.attach(generate_invoke("log.msg.debug", "db.clear", frame));

			auto tpl = cyng::tuple_cast<
				std::string			//	[0] table name
			>(frame);

			ctx.attach(generate_invoke("log.msg.debug", "db.clear", std::get<0>(tpl)));

			db.clear(std::get<0>(tpl), ctx.tag());
			//{
			//	ctx.attach(generate_invoke("log.msg.warning", "db.clear - failed", std::get<0>(tpl)));
			//}
		}));

	}

}
