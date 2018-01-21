/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/dom/reader.h>
#include <cyng/object_cast.hpp>
#include <cyng/core/class_interface.h>
#include <cyng/intrinsics/traits/tag.hpp>
#include <cyng/factory.h>

namespace cyng 
{
	reader<object>::reader ()
	: root_(make_object())
	{}
	
	reader<object>::reader(object const& root)
	: root_(root)
	{}
	
	/**
		* @return true if current element is a leaf node and contains
		* without childrens.
		*/
	bool reader<object>::is_leaf() const
	{
		switch (root_.get_class().tag())
		{
			case TC_TUPLE:
			case TC_VECTOR:
			case TC_SET:
			case TC_ATTR_MAP:
			case TC_PARAM_MAP:
			case TC_ATTR:
			case TC_PARAM:
				return false;
			default:
				break;
		}
		return true;
	}
	
	object reader<object>::get(std::string const& name) const
	{
		return find(root_, name);
	}
	
	object reader<object>::get(std::size_t idx) const
	{
		return find(root_, idx);
	}
	
	
	reader<object> reader<object>::operator[](std::string const& name) const
	{
		return reader(get(name));
	}

	
	reader<object> reader<object>::operator[](std::size_t idx) const
	{
		return reader(get(idx));
	}
	
	//
	//	tuple
	//
	tuple_reader::tuple_reader(tuple_t const& data)
	: reader<tuple_t>(data)
	{}
	
	
	bool tuple_reader::is_leaf() const
	{
		return false;
	}
	
	//
	//	vector
	//
	vector_reader::vector_reader(vector_t const& data)
	: reader<vector_t>(data)
	{}
	
	bool vector_reader::is_leaf() const
	{
		return false;
	}
	
	reader<object> vector_reader::operator[](std::size_t idx) const
	{
		return reader<object>(get(idx));
	}
	
	//
	//	set
	//
	set_reader::set_reader(set_t const& data)
	: reader<set_t>(data)
	{}
	
	bool set_reader::is_leaf() const
	{
		return false;
	}
}





