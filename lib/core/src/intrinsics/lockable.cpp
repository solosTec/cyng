/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/lockable.h>


namespace cyng 
{
	lockable::lockable()
	: obj_()
	, m_()
	, generation_(0)
	{}
	
	lockable::lockable(object&& obj, std::uint64_t generation)
	: obj_(std::move(obj))
	, m_()
	, generation_(generation)
	{}
	
	lockable::lockable(lockable&& l)
	: obj_(std::move(l.obj_))
	, m_()
	, generation_(l.generation_)
	{}
	
	lockable& lockable::operator=(lockable const& other)
	{
		if (this != &other)
		{
			async::unique_lock<async::shared_mutex>	this_lock(m_, async::defer_lock);
			async::unique_lock<async::shared_mutex>	other_lock(other.m_, async::defer_lock);
			async::lock(this_lock, other_lock);
			obj_ = other.obj_;
			generation_ = other.generation_;
		}
		return *this;
	}
	
	lockable& lockable::operator=(lockable&& other)
	{
		obj_ = std::move(other.obj_);
		std::swap(generation_, other.generation_);
		return *this;
	}
	
	
}



