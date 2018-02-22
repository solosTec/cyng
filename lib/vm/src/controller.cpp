/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/controller.h>
#include <cyng/factory.h>
//#include <future>
#include <memory>
#include <boost/assert.hpp>
#ifdef _DEBUG
#include <cyng/io/serializer.h>
#endif
#include <boost/functional/hash.hpp>

namespace cyng 
{

	controller::controller(io_service_t& ios, boost::uuids::uuid tag, std::ostream& out, std::ostream& err)
	: dispatcher_(ios)
	, vm_(tag, out, err)
	, halt_(false)
	, mutex_()
	{}

	controller const& controller::run(vector_t&& prg) const
	{
		if (!halt_)	
		{
			execute(std::move(prg), async::sync());
		}
		return *this;
	}
	
	controller const& controller::async_run(vector_t&& prg) const
	{
		if (!halt_)	
		{
			execute(std::move(prg), async::detach());
		}
		return *this;
	}
	
	void controller::execute(vector_t&& prg, async::sync) const
	{
		//
		//	recursive calls are not allowed
		//
//		if (dispatcher_.running_in_this_thread())
//		{
//			std::cerr 
//				<< '\n'
//				<< '\n'
//				<< "***error: recursion(sync)! - "
//				<< std::this_thread::get_id()
//				<< '\n'
//				<< '\n'
//				<< prg.size()
//				<< " op(s)"
//				<< std::endl
//				;
//#ifdef _DEBUG
//			std::cerr << io::to_str(prg) << std::endl;
//#endif
//			return;
//		}

		
		//
		//	grab the content of the code vector 
		//
		prg_param param(std::move(prg));
		
		//
		//	prepare condition variable
		//
		std::condition_variable cv;
		async::unique_lock<async::mutex> lock(mutex_);
		bool complete = false;	//	bullet proof

		dispatcher_.post([this, param, &cv, &complete]() {

			this->vm_.run(std::move(param.prg_));
			
			//
			//	set condition
			//
			complete = true;
			cv.notify_one();

		});
		
		//
		//	wait for condition 
		//
		cv.wait(lock, [&complete] { return complete; });

	}
	
	void controller::execute(vector_t&& prg, async::detach) const
	{
		//
		//	grab the content of the code vector 
		//
		prg_param param(std::move(prg));
		dispatcher_.post([this, param](){
			
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






