/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/controller.h>
#include <cyng/factory.h>
#include <memory>
#include <boost/assert.hpp>
#ifdef _DEBUG
#include <cyng/io/serializer.h>
#endif
#include <system_error>
//#include <thread>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng 
{

	controller::controller(io_service_t& ios, boost::uuids::uuid tag, std::ostream& out, std::ostream& err)
	: dispatcher_(ios)
	, vm_(tag, out, err)
	, halt_(false)
	, mutex_()
	, call_stack_()
	{}

	controller const& controller::run(vector_t&& prg) const
	{
		if (!halt_)	
		{
#if (BOOST_ASIO_VERSION < 101200)
			BOOST_ASSERT_MSG(!dispatcher_.stopped(), "service not running");
#else
			BOOST_ASSERT_MSG(!dispatcher_.stopped(), "service not running");
#endif
			execute(std::move(prg), async::sync());
		}
		return *this;
	}
	
	controller const& controller::async_run(vector_t&& prg) const
	{
		if (!halt_)	
		{
#if (BOOST_ASIO_VERSION < 101200)
			BOOST_ASSERT_MSG(!dispatcher_.stopped(), "service not running");
#else
			BOOST_ASSERT_MSG(!dispatcher_.stopped(), "service not running");
#endif
			execute(std::move(prg), async::detach());
		}
		return *this;
	}
	
	void controller::execute(vector_t&& prg, async::sync) const
	{
		try
		{
			//
			//	ToDo: test recursion
			//call_stack_.running_in_this_thread();
			//	if running in same thread use the append method 
			//

			//
			//	prepare lock
			//
			async::lock_guard<async::mutex> lock(mutex_);
			alloc_stack alloc(call_stack_);
			this->vm_.run(std::move(prg));
		}
		catch (const std::system_error& ex)
		{
			//
			//	recursion
			//
			std::cerr
				<< "\n\n***ERROR recursion\t"
				<< this->vm_.tag()
				<< "\n\n"
				<< ex.what()
				<< std::endl;
		}
		catch (...)
		{
			std::cerr
				<< "\n\n***FATAL exception\t"
				<< this->vm_.tag()
				<< "\n\n"
				<< std::endl;
		}
					
	}
	
	void controller::execute(vector_t&& prg, async::detach) const
	{
		//
		//	grab the content of the code vector 
		//
		prg_param param(std::move(prg));
		dispatcher_.post([this, param](){
			
			//
			//	prepare lock
			//
			async::lock_guard<async::mutex> lock(mutex_);
			alloc_stack alloc(call_stack_);
			this->vm_.run(std::move(param.prg_));

		});
	}
	
	void controller::halt()
	{
		//
		//	set halt flag
		//
		halt_ = true;
		
		//
		//	last instruction - clears library
		//
		execute(vector_t{ make_object(code::HALT) }, async::sync());
	}

	std::size_t controller::hash() const noexcept
	{
		static boost::hash<boost::uuids::uuid> uuid_hasher;
		return uuid_hasher(tag());
	}

	boost::uuids::uuid controller::tag() const noexcept
	{
		return vm_.tag();
	}

	call_stack::call_stack()
		: call_stack_()
	{}

	bool call_stack::running_in_this_thread() const
	{
		return (call_stack_.empty())
			? false
			: (call_stack_.top() == std::this_thread::get_id())
			;
	}

	alloc_stack::alloc_stack(call_stack const& cs)
		: call_stack_(cs)
	{
		call_stack_.call_stack_.push(std::this_thread::get_id());
	}
	alloc_stack::~alloc_stack()
	{
		call_stack_.call_stack_.pop();
	}


	prg_param::prg_param(vector_t&& prg)
		: prg_(std::move(prg))
	{}

	prg_param::prg_param(prg_param const& other)
		: prg_(std::move(other.prg_))
	{}

	prg_param::prg_param(prg_param&& other)
		: prg_(std::move(other.prg_))
	{}

}






