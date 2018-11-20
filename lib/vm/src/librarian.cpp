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

#ifdef _DEBUG
#include <iostream>
#endif
#include <cyng/factory.h>

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

	bool librarian::invoke(std::string const& name, context& ctx) const
	{
		auto pos = db_.find(name);
		if (pos != db_.end())
		{
			//	test stack size 
			const std::size_t arity = (*pos).second.arity();
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

					//	print program to execute
					ss
						<< "*** program("
						<< ctx.mem_.level()
						<< "%): "
						<< ctx.mem_;

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
					<< cyng::io::to_str(ctx.get_frame())
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
		//	function not registered
		//
		return false;
	}
	
	void librarian::register_this()
	{
		//
		//	insert a function
		//
        insert("lib.insert", 3, [this](context& ctx){
			
            const vector_t frame = ctx.get_frame();
			
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
			
			const vector_t frame = ctx.get_frame();
			const std::string name = value_cast<std::string>(frame[0], "");
			this->erase(name);

		});

		//
		//	Even an invoke is possible (but with the same
		//	context - and therefore complete experimental)
		//
		insert("lib.invoke", 1, [this](context& ctx){
			
			const vector_t frame = ctx.get_frame();
			const std::string name = value_cast<std::string>(frame[0], "");
 
			//	standard function call (no out of band)
			this->invoke(name, ctx);
			
		});
		
		//
		//	print some output for testing purpose
		//
		insert("lib.test", 1, [this](context& ctx){
			
			vector_t frame = ctx.get_frame();
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
			vector_t prg;
			memory mem(std::move(prg));

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
			vector_t prg;
			memory mem(std::move(prg));

			//
			//	save and restore call stack 
			//
			activation a(v.stack_);
			v.stack_.push(make_object(msg));
			context ctx_log(v, mem, "log.msg.debug");
			(*pos).second(ctx_log);
			return true;
		}
		return false;
	}

	bool librarian::try_halt(vm& v) const
	{
		auto pos = db_.find("vm.halt");
		if (pos != db_.end())
		{
			//
			//	fake memory
			//
			vector_t prg;
			memory mem(std::move(prg));

			//
			//	save and restore call stack 
			//
			activation a(v.stack_);
			context ctx_halt(v, mem, "vm.halt");
			(*pos).second(ctx_halt);
			return true;
		}
		return false;
	}

}







