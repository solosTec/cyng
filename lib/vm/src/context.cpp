/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/context.h>
#include <cyng/vm/vm.h>
#include <cyng/vm/memory.h>

namespace cyng 
{
	context::context(vm& v, memory& mem)
	: vm_(v)
	, mem_(mem)
	{}
	
	context::context(context& ctx, memory& mem)
		: vm_(ctx.vm_)
		, mem_(mem)
	{}

	vector_t context::get_frame() const
	{
		return vm_.stack_.get_frame();
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
	void context::push(object const& obj)
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
	
	void context::run(vector_t&& prg)
	{
		vm_.sync_run(std::move(prg));
	}

	context& context::attach(vector_t&& prg)
	{
		mem_ += std::move(prg);
		return *this;
	}

	namespace traits
	{
	
#if defined(CYNG_LEGACY_MODE_ON)
		const char type_tag<cyng::vm_call>::name[] = "vm-call";
#endif
	}	// traits		
}

namespace std
{
	size_t hash<cyng::vm_call>::operator()(cyng::vm_call const& c) const noexcept
	{
		return hash<size_t>{}(get_target_address(c));
	}

	bool equal_to<cyng::vm_call>::operator()(cyng::vm_call const& c1, cyng::vm_call const& c2) const noexcept
	{
		return get_target_address(c1) == get_target_address(c2);
	}

	bool less<cyng::vm_call>::operator()(cyng::vm_call const& c1, cyng::vm_call const& c2) const noexcept
	{
		return get_target_address(c1) < get_target_address(c2);
	}

}
