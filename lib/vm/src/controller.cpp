/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/vm/controller.h>
#include <cyng/factory.h>
#include <future>
#include <memory>
#include <boost/assert.hpp>
#ifdef _DEBUG
#include <cyng/io/serializer.h>
#endif
#include <boost/functional/hash.hpp>

namespace cyng 
{

	controller::controller(boost::asio::io_service& ios, boost::uuids::uuid tag, std::ostream& out, std::ostream& err)
	: dispatcher_(ios)
	, vm_(tag, out, err)
	, halt_(false)
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
		vector_t* vec = new vector_t(std::move(prg));
// 		std::unique_ptr<vector_t> vec = std::unique_ptr<vector_t>(new vector_t(std::move(prg)));
		
		std::promise<bool> result;
		auto f = result.get_future();
		
		dispatcher_.post([this, vec, &result](){
			
			this->vm_.run(std::move(*vec));
			delete vec;
			
			//
			//	set condition
			//
			result.set_value(true);
			
		});
		
		//
		//	wait for condition 
		//
		f.wait();
		
	}
	
	void controller::execute(vector_t&& prg, async::detach) const
	{
		//
		//	grab the content of the code vector 
		//
// 		std::unique_ptr<vector_t> vec = std::unique_ptr<vector_t>(new vector_t(std::move(prg)));
		vector_t* vec = new vector_t(std::move(prg));
		dispatcher_.post([this, vec](){
			
			this->vm_.run(std::move(*vec));
			delete vec;
			
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
		boost::hash<boost::uuids::uuid> uuid_hasher;
		return uuid_hasher(vm_.tag());

	}

	
}






