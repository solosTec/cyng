/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/context.h>
#include <cyng/vm/vm.h>

namespace cyng 
{
	context::context(vm& v)
	: vm_(v)
	{}
	
	vector_t context::get_frame(bool remove)
	{
		return vm_.stack_.get_frame(remove);
	}
	
	std::size_t context::frame_size() const noexcept
	{
		return vm_.stack_.frame_size();
	}
	
	boost::uuids::uuid context::tag() const noexcept
	{
		return vm_.tag();
	}
	
	void context::set_return_value(object const& obj, std::size_t idx)
	{
		vm_.stack_.setr(obj, idx);
	}
	
	void context::set_return_value()
	{
		vm_.stack_.setr();
	}

	void context::push(object&& obj)
	{
		vm_.stack_.push(std::move(obj));
	}
	
	void context::set_register(boost::system::error_code ec)
	{
		//
		//	debug output
		//
		if (ec)
		{
#ifdef _DEBUG
			std::cerr
				<< "set_register("
				<< ec
				<< ": "
				<< ec.message()
				<< ")"
				<< std::endl
				;
#endif
		}
		
		vm_.error_register_ = ec;
	}
	
	void context::set_register(boost::system::errc::errc_t code)
	{
		set_register(boost::system::errc::make_error_code(code));
	}

	void context::clear_register()
	{
		set_register(boost::system::error_code());
	}
	
	namespace traits
	{
	
#if defined(CYNG_LEGACY_MODE_ON)
		const char type_tag<cyng::vm_call>::name[] = "vm-call";
#endif
	}	// traits		
}








