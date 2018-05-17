/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/controller.h>
#include <cyng/factory.h>
#include <cyng/value_cast.hpp>
#include <cyng/io/serializer.h>

#include <memory>
#include <boost/assert.hpp>
#ifdef _DEBUG
#include <cyng/io/serializer.h>
#endif
#include <system_error>
#include <boost/functional/hash.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace cyng 
{

	controller::controller(io_service_t& ios
		, boost::uuids::uuid tag
		, std::ostream& out
		, std::ostream& err)
	: dispatcher_(ios)
		, vm_(tag, out, err)
		, halt_(false)
		, mutex_()
		, call_stack_()
		, load_(0)
	{}

	controller const& controller::run(vector_t&& prg) const
	{
		if (!halt_)	
		{
#ifdef CYNG_VM_SIMPLE_LOCK
			BOOST_ASSERT_MSG(!dispatcher_.stopped(), "service not running");
#else
#if (BOOST_ASIO_VERSION < 101200)
			BOOST_ASSERT_MSG(!dispatcher_.get_io_service().stopped(), "service not running");
#else
			BOOST_ASSERT_MSG(!dispatcher_.get_io_context().stopped(), "service not running");
#endif
#endif
			execute(std::move(prg), async::sync());
		}
		return *this;
	}
	
	controller const& controller::async_run(vector_t&& prg) const
	{
		if (!halt_)	
		{
#ifdef CYNG_VM_SIMPLE_LOCK
			BOOST_ASSERT_MSG(!dispatcher_.stopped(), "service not running");
#else
#if (BOOST_ASIO_VERSION < 101200)
			BOOST_ASSERT_MSG(!dispatcher_.get_io_service().stopped(), "service not running");
#else
			BOOST_ASSERT_MSG(!dispatcher_.get_io_context().stopped(), "service not running");
#endif
#endif
			execute(std::move(prg), async::detach());
		}
		return *this;
	}
	
	void controller::execute(vector_t&& prg, async::sync) const
	{
		//
		//	simple lock has no guaranties about execution ordering
		//
#ifdef CYNG_VM_SIMPLE_LOCK
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
#else
		if (dispatcher_.running_in_this_thread())
		{
			//
			//	recursion is not allowed
			//
			std::cerr
				<< "\n\n***error: recursion("
				<< vm_.tag()
				<< ")! - "
				<< std::this_thread::get_id()
				<< "\n\n"
				<< prg.size()
				<< " op(s)"
				<< std::endl
				;
			BOOST_ASSERT_MSG(false, "VM recursion");
			return;
		}


		//
		//	move the content of the code vector 
		//
		vector_t dump(prg);	//	make a copy
		parameter param(std::move(prg));

		//
		//	prepare condition variable
		//
		//async::condition_variable cv;
		//async::unique_lock<async::mutex> lock(mutex_);
		//bool complete = false;	//	bullet proof
		++load_;

		std::atomic<bool> complete{ false };	//	completion flag

		dispatcher_.dispatch([this, param, &complete]() {

			this->vm_.run(std::move(param.prg_));
			//
			//	set condition
			//
			--load_;
			complete.exchange(true);
		});

		//
		//	wait for flag
		//
		std::size_t counter{ 0 };
		while (!complete.load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			++counter;
			if (counter > 16)
			{
				std::cerr
					<< "\n\n"
					<< "*** "
					<< this->vm_.tag()
					<< " sync-load: "
					<< load_.load()
					<< " / "
					<< std::dec
					<< counter
					<< " / "
					<< io::to_str(dump)
					<< " ***\n\n"
					<< std::endl
					;
				break;
			}
		}

		//dispatcher_.post([this, param, &cv/*, &complete*/]() {

		//	this->vm_.run(std::move(param.prg_));

		//	//
		//	//	set condition
		//	//
		//	//async::lock_guard<async::mutex> lk(mutex_);
		//	//complete = true;
		//	cv.notify_all();
		//	--load_;

		//});

		//
		//	wait for condition 
		//
		//cv.wait(lock, [&complete] { return complete; });
		//cv.wait(lock);

		
#endif	//	CYNG_VM_SIMPLE_LOCK

	}
	
	void controller::execute(vector_t&& prg, async::detach) const
	{
#ifdef CYNG_VM_SIMPLE_LOCK

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
#else
		//
		//	grab the content of the code vector 
		//
		parameter param(std::move(prg));
		dispatcher_.post([this, param]() {

			this->vm_.run(std::move(param.prg_));

		});

#endif
	}
	
	void controller::halt()
	{
		//
		//	set halt flag
		//
		halt_ = true;
		
        std::atomic<bool> complete{ false };	//	completion flag

		dispatcher_.dispatch([this, &complete]() {

			//
			//	last instruction - clears library
			//
			this->vm_.run(vector_t{ make_object(code::HALT) });

			//
			//	set condition
			//
			complete.exchange(true);

		});

		//
		//	wait for flag
		//
		while (!complete.load())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

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

	bool controller::same_thread() const
	{
		return dispatcher_.running_in_this_thread();
	}

	void controller::register_function(std::string name
		, std::size_t arity
		, vm_call proc)
	{
		dispatcher_.post([this, name, arity, proc]() {

			this->vm_.lib_.insert(name, arity, proc);

		});
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


	controller::parameter::parameter(vector_t&& prg)
		: prg_(std::move(prg))
	{}

	controller::parameter::parameter(parameter const& other)
		: prg_(std::move(other.prg_))
	{}

	controller::parameter::parameter(parameter&& other)
		: prg_(std::move(other.prg_))
	{}

	std::size_t op_counter(vector_t const& prg, code op)
	{
		return std::count_if(prg.begin(), prg.end(), [&](object const& obj) {
			return ((obj.get_class().tag() == TC_CODE) && (value_cast(obj, code::NOOP) == op));
		});
	}

}






