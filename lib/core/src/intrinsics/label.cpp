/*
 * The MIT License (MIT)
 * 
 * Copyright (c) 2018 Sylko Olzscher 
 * 
 */ 

#include <cyng/intrinsics/label.h>


namespace cyng 
{
	label::label()
		: name_()
	{}

	label::label(label const& other)
		: name_(other.name_)
	{}

	label::label(std::string const& name)
		: name_(name)
	{}

	//	comparison
	bool operator==(label const& lhs, label const& rhs)
	{
		return lhs.name_ == rhs.name_;
	}
	bool operator<(label const& lhs, label const& rhs)
	{
		return lhs.name_ < rhs.name_;
	}
	bool operator!=(label const& lhs, label const& rhs)
	{
		return !(lhs == rhs);
	}
	bool operator>(label const& lhs, label const& rhs)
	{
		//	note the reversed notation
		return rhs < lhs;
	}
	bool operator<=(label const& lhs, label const& rhs)
	{
		return !(lhs > rhs);
	}
	bool operator>=(label const& lhs, label const& rhs)
	{
		return !(lhs < rhs);
	}

}





