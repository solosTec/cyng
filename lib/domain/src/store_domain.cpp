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
#include <algorithm>

namespace cyng 
{

	void register_store(store::db& db, context& ctx)
	{
		ctx.run(register_function("db.insert", 4, [&db](context& ctx) {
			const vector_t frame = ctx.get_frame();
			const std::string name = value_cast<std::string>(frame.at(0), "");

			vector_t tmp;
			table::key_type key = value_cast(frame.at(1), tmp);
			std::reverse(key.begin(), key.end());

			table::data_type data = value_cast(frame.at(2), tmp);
			std::reverse(data.begin(), data.end());

			const std::uint64_t generation = value_cast<std::uint64_t>(frame.at(3), 0);
			if (!db.insert(name, key, data, generation))
			{
				ctx.run(generate_invoke("log.msg.warning", "db.insert failed", key, data));
			}
			//ctx.set_return_value(make_object(s.is_open()), 0);
		}));

	}

}
