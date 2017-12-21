/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/librarian.h>
#include <cyng/value_cast.hpp>
#include <cyng/io/serializer.h>

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
	
	bool librarian::invoke(std::string const& name, context& ctx) const
	{
		auto pos = db_.find(name);
		if (pos != db_.end())
		{
			//	test stack size 
			const std::size_t arity = (*pos).second.arity();
			if (ctx.frame_size() >= arity)
			{
				(*pos).second(ctx);
				return true;
			}
			else 
			{
				std::cerr 
					<< "\n***Warning: insufficient frame size: "
					<< ctx.frame_size()
					<< '/'
					<< arity 
					<< " ["
					<< name
					<< "]\n"
					<< std::endl 
					;
			}
		}
		return false;
	}
	
	void librarian::register_this()
	{
		//
		//	insert a function
		//
		insert("lib.insert", 3, [this](context& ctx){
			
			const vector_t frame = ctx.get_frame(true);
			
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
			
			const vector_t frame = ctx.get_frame(true);
			const std::string name = value_cast<std::string>(frame[0], "");
			this->erase(name);

		});

		//
		//	Even an invoke is possible (but with the same
		//	context - and therefore complete experimental)
		//
		insert("lib.invoke", 1, [this](context& ctx){
			
			const vector_t frame = ctx.get_frame(true);
			const std::string name = value_cast<std::string>(frame[0], "");
 
			//	standard function call (no out of band)
			this->invoke(name, ctx);
			
		});
		
		//
		//	print some output for testing purpose
		//
		insert("lib.test", 1, [this](context& ctx){
			
			vector_t frame = ctx.get_frame(true);
			std::cerr 
			<< "lib.test("
			<< frame.size()
			<< ", "
			;
			io::serialize_plain(std::cout, make_object(frame));
// 			<< value_cast<std::string>(frame[0], "")
			std::cerr
			<< ")"
			<< std::endl;
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
}







