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
#include <cyng/value_cast.hpp>
#include <cyng/tuple_cast.hpp>
#include <algorithm>

namespace cyng 
{

	void register_store(store::db& db, context& ctx)
	{
		ctx.attach(register_function("db.insert", 4, [&db](context& ctx) {

			const vector_t frame = ctx.get_frame();
			ctx.attach(generate_invoke("log.msg.debug", "db.insert", frame));

			auto tpl = cyng::tuple_cast<
				std::string,			//	[0] table name
				vector_t,				//	[1] key
				vector_t,				//	[2] data
				std::uint64_t			//	[3] generation
				//boost::uuids::uuid		//	[4] source
			>(frame);

			//BOOST_ASSERT(ctx.tag() == std::get<4>(tpl));

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
				ctx.attach(generate_invoke("log.msg.warning", "db.insert - failed", std::get<0>(tpl), std::get<1>(tpl)));
			}
		}));

		ctx.attach(register_function("db.modify.by.attr", 3, [&db](context& ctx) {
			const vector_t frame = ctx.get_frame();

			ctx.attach(generate_invoke("log.msg.debug", "db.modify", frame));

			auto tpl = cyng::tuple_cast<
				std::string,			//	[0] table name
				vector_t,				//	[1] key
				attr_t					//	[2] data
				//boost::uuids::uuid		//	[3] source
			>(frame);

			//BOOST_ASSERT(ctx.tag() == std::get<3>(tpl));

			//
			//	key
			//
			std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

			if (!db.modify(std::get<0>(tpl), std::get<1>(tpl), std::move(std::get<2>(tpl)), ctx.tag()))
			{
				ctx.attach(generate_invoke("log.msg.warning", "db.modify - failed", std::get<0>(tpl), std::get<1>(tpl)));
			}
		}));

		ctx.attach(register_function("db.modify.by.param", 3, [&db](context& ctx) {
			const vector_t frame = ctx.get_frame();

			ctx.attach(generate_invoke("log.msg.debug", "db.modify", frame));

			auto tpl = cyng::tuple_cast<
				std::string,			//	[0] table name
				vector_t,				//	[1] key
				param_t				//	[2] data
				//boost::uuids::uuid		//	[3] source
			>(frame);

			//BOOST_ASSERT(ctx.tag() == std::get<3>(tpl));

			//
			//	key
			//
			std::reverse(std::get<1>(tpl).begin(), std::get<1>(tpl).end());

			if (!db.modify(std::get<0>(tpl), std::get<1>(tpl), std::move(std::get<2>(tpl)), ctx.tag()))
			{
				ctx.attach(generate_invoke("log.msg.warning", "db.modify - failed", std::get<0>(tpl), std::get<1>(tpl)));
			}
		}));
	}

}
