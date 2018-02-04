/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/reference.h>
#include <cyng/object.h>

namespace cyng 
{
	reference::reference(object const& obj)
	: ref_(obj.value_)
	{}
	
	long reference::use_count() const
	{
		return ref_.use_count() -  1;
	}	
}



