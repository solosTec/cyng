/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 
#include <cyng/object.h>
#include <cyng/core/class_interface.h>
#include <cyng/core/object_interface.h>
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
	{
// 		std::cout << "object::object(object const& obj) = ";
// 		obj.serialize(std::cout)	<< " => ";
// 		this->serialize(std::cout)	<< std::endl;
		
	}
	
	object::object(object&& obj)
 	: value_(std::move(obj.value_))
	{
// 		std::cout << "object::object(object&& obj)" << std::endl;
	}
	
	object::~object() noexcept
	{
// 		std::cout << "~operator(object const& obj) ";
// 		if (value_)	value_->serialize(std::cout); else std::cout << "NULL";
// 		std::cout<< std::endl;		
	}
	
	object& object::operator=(object const& obj)
	{
// 		std::cout << "operator=(object const& obj)" << std::endl;
		value_.operator=(obj.value_);
		return *this;
	}
	
	object& object::operator=(object&& obj)
	{
// 		std::cout << "operator=(object&& obj)" << std::endl;
		value_.operator=(std::move(obj.value_));
		return *this;		
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
}

namespace std 
{
	size_t hash<cyng::object>::operator()(cyng::object const& obj) const noexcept
	{
		return obj.hash();
	}
}
