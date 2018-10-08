/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include <cyng/object.h>
#include <cyng/core/class_interface.h>
#include <cyng/core/object_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <CYNG_project_info.h>

namespace cyng 
{
	object::object()
	: value_()
	{}
	
	object::object(core::shared_object so)
	: value_(std::move(so))
	{}
	
	object::object(object const& obj)
	: value_(obj.value_)
	{}
	
	object::object(object&& obj)
 	: value_(std::move(obj.value_))
	{}
	
	object::~object() noexcept
	{}
	
	object& object::operator=(object const& obj)
	{
		value_.operator=(obj.value_);
		return *this;
	}
	
	object& object::operator=(object&& obj)
	{
		value_.operator=(std::move(obj.value_));
		return *this;		
	}
	
	bool object::is_null() const noexcept
	{
		//
		//	First test if there is a value and then if this value
		//	is of type null.
		//
		return (!!value_ && (value_->get_class().tag() == TC_NULL));
	}

	object::operator bool() const noexcept
	{
		//	force bool() operator
		return !!value_;
	}
	
	core::class_interface const& object::get_class () const noexcept
	{
		return value_->get_class();
	}
	
	void object::clear()
	{
		value_.reset();
	}
	
	std::size_t object::hash() const noexcept
 	{
 		return (value_)
 		? value_->hash()
 		: CYNG::null_hash;
 	}
	
	//	swap
	void swap (object& lhs, object& rhs) noexcept
	{
		 lhs.value_.swap( rhs.value_ );
	}
	
	bool equal_by_instance(object const& o1, object const& o2) noexcept
	{
		return o1.value_.get() == o2.value_.get();
	}
	
	bool equal_by_value(object const& o1, object const& o2) noexcept
	{
		return o1.value_->equal_to(o2);
	}
	
	bool operator==(object const& o1, object const& o2)
	{
		return equal_by_value(o1, o2);
	}

	bool operator!=(object const& o1, object const& o2)
	{
		return !equal_by_value(o1, o2);
	}

	bool less_by_value(object const& o1, object const& o2) noexcept
	{
		return o1.value_->less(o2);
	}

	bool operator<(object const& o1, object const& o2)
	{
		return less_by_value(o1, o2);
	}

}

namespace std 
{
	size_t hash<cyng::object>::operator()(cyng::object const& obj) const noexcept
	{
		return obj.hash();
	}
}
