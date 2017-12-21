/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2017 Sylko Olzscher 
 * 
 */ 

#include <cyng/tracker.h>
#include <cyng/object.h>

namespace cyng 
{
	tracker::tracker(object const& obj)
	: ref_(obj.value_)
	{}
	
	bool tracker::expired() const
	{
		return ref_.expired();
	}

	long tracker::use_count() const
	{
		return ref_.use_count();
	}	
}



