/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/librarian.h>
#include <cyng/value_cast.hpp>
#include <cyng/io/serializer.h>
#include <cyng/vm/memory.h>
#include <cyng/vm/manip.h>
#include <cyng/vm/vm.h>
#include <cyng/vm/generator.h>
#include <cyng/factory.h>

#ifdef _DEBUG
#include <iostream>
#include <boost/algorithm/string.hpp>
#endif
#include <boost/uuid/uuid_io.hpp>


namespace cyng 
{
	librarian::librarian()
	: db_()
	{
		register_this();
	}

	void librarian::clear()
	{
		db_.clear();
	}
	
	bool librarian::erase(std::string const& name)
	{
		return db_.erase(name) != 0;
	}
	
	std::size_t librarian::size() const noexcept
	{
		return db_.size();
	}

	bool librarian::invoke(std::string const& name, vm& v, memory& mem) const
	{
		auto pos = db_.find(name);
		if (pos != db_.end())
		{
			//	test stack size 
			auto const arity = (*pos).second.arity();
			context ctx(v, mem, name, arity);
			if (ctx.frame_size() >= arity)
			{
				try
				{
					(*pos).second(ctx);
					return true;
				}
				catch (std::exception const& ex)
				{
					//
					//	create an error message
					//
					std::stringstream ss;
					ss
						<< "***error("
						<< name
						<< "): "
						<< ex.what()
						;
					std::string msg = ss.str();
					if (!try_error_log(ctx, msg))
					{
						//
						//	print to standard error output if no logger available.
						//
						std::cerr
							<< "\n\n"
							<< msg
							<< "\n\n"
							<< std::endl;
					}

					//	reset
					ss.str("");

					//
					//	print stack dump
					//
					ctx.vm_.stack_.dump(ss);
					msg = ss.str();
					if (!try_error_log(ctx, msg))
					{
						//
						//	print to standard error output if no logger available.
						//
						std::cerr
							<< "\n\n"
							<< msg
							<< "\n\n"
							<< std::endl;
					}
				}
			}
			else 
			{
				std::stringstream ss;
				ss
					<< "***Warning: insufficient frame size: "
					<< ctx.frame_size()
					<< '/'
					<< arity 
					<< " ["
					<< name
					<< "] "
					<< io::to_type(ctx.get_frame())
					;
				const std::string msg = ss.str();
				if (!try_error_log(ctx, msg))
				{
					std::cerr
						<< "\n\n"
						<< msg
						<< "\n\n"
						<< std::endl
						;
				}
			}

			//
			//	error - but function is available
			//	
			return true;
		}

		//
		//	error: function not registered
		//
		context ctx(v, mem, name, 0);

		auto const frame = ctx.get_frame();
		std::stringstream ss;
		ss
			<< "***Warning: function ["
			<< name
			<< "] is not registered in VM "
			<< v.tag()
			<< " - "
			<< cyng::io::to_type(frame)
			;

		const std::string msg = ss.str();
		if (!try_error_log(ctx, msg))
		{
			std::cerr
				<< "\n\n"
				<< msg
				<< "\n\n"
				<< std::endl
				;
		}

		return false;
	}
	
	void librarian::register_this()
	{
		//
		//	insert a function
		//
        insert("lib.insert", 3, [this](context& ctx){
			
			auto const frame = ctx.get_frame();

            const vm_call fun = value_cast(frame[0], vm_call());
            const std::size_t arity = value_cast<std::size_t>(frame[1], 0u);
            const std::string name = value_cast<std::string>(frame[2], "");
            this->insert(name, arity, fun);
        });
		
		//
		//	clear library
		//
		insert("lib.clear", 0, std::bind(&librarian::clear, this));
		
		//
		//	delete a specific function
		//
		insert("lib.erase", 1, [this](context& ctx){
			
			auto const frame = ctx.get_frame();
			const std::string name = value_cast<std::string>(frame[0], "");
			this->erase(name);

		});

		//
		//	Even an invoke is possible (but with the same
		//	context - and therefore complete experimental)
		//
		insert("lib.invoke", 1, [this](context& ctx){
			
			auto const frame = ctx.get_frame();
			auto const name = value_cast(frame[0], "");
 
			//	standard function call (no out of band)
			memory mem(vector_t{});
			this->invoke(name, ctx.vm_, mem);

		});
		
		//
		//	print some output for testing purpose
		//
		insert("lib.test", 1, [this](context& ctx){
			
			auto const frame = ctx.get_frame();
			std::cerr 
			<< "lib.test("
			<< frame.size()
			<< ", "
			;
			io::serialize_plain(std::cout, make_object(frame));
			std::cerr
			<< ")"
			<< std::endl;
		});
		
		//
		//	Push number of registered callbacks on stack
		//
		insert("lib.size", 0, [this](context& ctx) {

			ctx.push(make_object(db_.size()));

		});

		//
		//	Take all parameters from stack and build a vector.
		//	Also empty vectors allowed.
		//	Memory has to be reserved (ASP) on the stack 
		//	to hold the return value.
		//
		insert("lib.make.vec", 0, [this](context& ctx) {

			ctx.push(make_object(ctx.get_frame()));
			ctx.set_return_value();

		});

		//
		//	Take all parameters from stack and build a vector.
		//	Also empty vectors allowed.
		//
		insert("lib.forward", 2, [this](context& ctx) {

			auto frame = ctx.get_frame();

			auto pos = frame.begin();
			BOOST_ASSERT(is_of_type<TC_UUID>(*pos));
			auto const tag = value_cast(*pos, boost::uuids::uuid());
			frame.erase(pos);

			pos = frame.begin();
			BOOST_ASSERT(is_of_type<TC_STRING>(*pos));
			auto const fn = value_cast(*pos, "");
			frame.erase(pos);

			vector_t vec;
			vec 
				<< code::ESBA
				<< unwind<vector_t>(frame)	//	non-recursive
				<< cyng::invoke(fn)	//	constructor
				<< code::REBA
				;

			ctx.forward(tag, vec);
			//ctx.forward(tag, generate_invoke(fn, unwind<vector_t>(frame)));

		});

	}
	
	bool librarian::insert(std::string const& name, procedure&& p)
	{
		auto pos = db_.find(name);
		if (pos == db_.end())
		{	
			return db_.emplace(std::piecewise_construct
				, std::forward_as_tuple(name)
				, std::forward_as_tuple(std::move(p))).second;
		}
		return false;
	}
	
	bool librarian::insert(std::string const& name
	, std::size_t arity
	, vm_call f)
	{
		return insert(name, procedure(f, arity));
	}

	bool librarian::try_error_log(context& ctx, std::string msg) const
	{
		// 
		auto pos = db_.find("log.msg.error");
		if (pos != db_.end())
		{
			//
			//	fake memory
			//
			memory mem(vector_t{});

			//
			//	save and restore call stack 
			//
			activation a(ctx.vm_.stack_);
			ctx.vm_.stack_.push(make_object(msg));

			context ctx_log(ctx, mem);
			(*pos).second(ctx_log);
			return true;
		}
		return false;
	}

	bool librarian::try_debug_log(vm& v, std::string msg) const
	{
		// 
		auto pos = db_.find("log.msg.debug");
		if (pos != db_.end())
		{
			//
			//	fake memory
			//
			memory mem(vector_t{});

			//
			//	save and restore call stack 
			//
			activation a(v.stack_);
			v.stack_.push(make_object(msg));
			context ctx_log(v, mem, "log.msg.debug", 1);
			(*pos).second(ctx_log);
			return true;
		}
		return false;
	}
}







